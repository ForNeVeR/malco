/**
 * @file sc_md5.h
 * @author impworks, Julienne Walker.
 * sc_map and sc_mapitem header.
 * Defines properties and methods of sc_map and sc_mapitem classes.
 * Much of this code has been adopted from http://www.eternallyconfuzzled.com
 */

#ifndef SC_MAP_H
#define SC_MAP_H

//--------------------------------
//  sc_mapitem
//--------------------------------

/**
 * sc_mapitem constructor.
 * @param key Item key.
 * @param val Item value.
 */
sc_mapitem::sc_mapitem(const char *key, const char *val)
{
  mBalance = 0;
  mLink[0] = mLink[1] = pRoot = NULL;
  pPrev = pNext = NULL;

  mKey = new ic_string(key);
  if(!mKey) ERROR(M_ERR_NO_MEMORY, M_EMODE_ERROR);
  mValue = new ic_string(val);
  if(!mValue) ERROR(M_ERR_NO_MEMORY, M_EMODE_ERROR);
}

/**
 * sc_mapitem destructor.
 */
sc_mapitem::~sc_mapitem()
{
  delete mKey;
  delete mValue;
}

//--------------------------------
//  sc_map
//--------------------------------

/**
 * sc_map constructor.
 */
sc_map::sc_map()
{
  mRoot = mCurr = mCurrKey = mFirst = mLast = NULL;
  mLength                                   = 0;
}

/**
 * sc_map copy constructor.
 * @param map sc_map to copy.
 */
sc_map::sc_map(sc_map &map)
{
  mRoot = mCurr = mCurrKey = mFirst = mLast = NULL;

  sc_mapitem *item;
  while(item = map.iter_next())
    set(item->mKey, item->mValue);

  map.iter_rewind();
}

/**
 * sc_map destructor.
 */
sc_map::~sc_map()
{
  clear();
}

/**
 * Adjusts balance of the node.
 * @param root Node to be processed.
 * @param dir Direction.
 * @param bal Balance value.
 */
inline void sc_map::adjust_balance(sc_mapitem *root, int dir, int bal)
{
  sc_mapitem *n  = root->mLink[dir];
  sc_mapitem *nn = n->mLink[!dir];

  if(nn->mBalance == 0)
    root->mBalance = n->mBalance = 0;
  else if(nn->mBalance == bal)
  {
    root->mBalance = -bal;
    n->mBalance    = 0;
  }
  else
  {
    root->mBalance = 0;
    n->mBalance    = bal;
  }

  nn->mBalance = 0;
}

/**
 * Performs a single rotation of the node
 * @param root Node to be rotated.
 * @param dir Direction.
 * @return Node that that has become root.
 */
inline sc_mapitem *sc_map::rotate_single(sc_mapitem *root, int dir)
{
  sc_mapitem *save = root->mLink[!dir];

  // provide back links
  // rule is simple:
  // if a->child = b, then b->father = a.
  root->mLink[!dir] = save->mLink[dir];
  if(save->mLink[dir]) save->mLink[dir]->pRoot = root;

  save->mLink[dir] = root;
  if(root) root->pRoot = save;

  return save;
}

/**
 * Performs a double rotation of the node
 * @param root Node to be rotated.
 * @param dir Direction.
 * @return Node that that has become root.
 */
inline sc_mapitem *sc_map::rotate_double(sc_mapitem *root, int dir)
{
  sc_mapitem *save = root->mLink[!dir]->mLink[dir];

  // rotation 1
  root->mLink[!dir]->mLink[dir] = save->mLink[!dir];
  if(save->mLink[!dir]) save->mLink[!dir]->pRoot = root->mLink[!dir];

  save->mLink[!dir] = root->mLink[!dir];
  if(root->mLink[!dir]) root->mLink[!dir]->pRoot = save;

  root->mLink[!dir] = save;
  if(save) save->pRoot = root;

  // rotation 2
  save = root->mLink[!dir];
  if(root->mLink[!dir]) root->mLink[!dir]->pRoot = save->pRoot;

  root->mLink[!dir] = save->mLink[dir];
  if(save->mLink[dir]) save->mLink[dir]->pRoot = root;

  save->mLink[dir] = root;
  if(root) root->pRoot = save;

  return save;
}

/**
 * Inserts a pair of key -> value to the map.
 * @param key Key if the item.
 * @param val Value of the item.
 * @return Pointer to newly inserted node.
 */
sc_mapitem *sc_map::insert(const char *key, const char *val)
{
  int done = 0;
  sc_mapitem *item;
  if(!key || !val) return NULL;
  mRoot        = this->insert_r(mRoot, key, val, &done, &item);
  mRoot->pRoot = NULL;

  // maintain linked list
  if(!mLength)
    mFirst = item;
  else
    mLast->pNext = item;
  item->pPrev = mLast;
  mLast       = item;

  iter_rewind();
  key_rewind();
  mLength++;
  return item;
}

/**
 * Recursive function behind insert.
 * @param root Root element to check for being an appropriate father.
 * @param key Key if the item.
 * @param val Value of the item.
 * @param done Flag to terminate recursion once it has completed.
 * @param item Pointer to newly inserted node.
 * @return Pointer to new root node.
 */
sc_mapitem *sc_map::insert_r(sc_mapitem *root, const char *key, const char *val, int *done, sc_mapitem **item)
{
  if(root == NULL)
  {
    root = new sc_mapitem(key, val);
    if(!root) ERROR(M_ERR_NO_MEMORY, M_EMODE_ERROR);
    *item = root;
  }
  else
  {
    int dir = (int)(root->mKey->compare(key) > 0);

    root->mLink[dir]        = insert_r(root->mLink[dir], key, val, done, item);
    root->mLink[dir]->pRoot = root;

    if(!*done)
    {
      root->mBalance += dir == 0 ? -1 : 1;

      if(root->mBalance == 0)
        *done = 1;
      else if(abs(root->mBalance) > 1)
      {
        root  = insert_balance(root, dir);
        *done = 1;
      }
    }
  }

  return root;
}

/**
 * Fixes balance after insert.
 * @param root Root element to fix balance for.
 * @param dir Direction.
 * @return Pointer to node that has become root
 */
inline sc_mapitem *sc_map::insert_balance(sc_mapitem *root, int dir)
{
  sc_mapitem *n = root->mLink[dir];
  int bal       = dir == 0 ? -1 : 1;

  if(n->mBalance == bal)
  {
    root->mBalance = n->mBalance = 0;
    root                         = rotate_single(root, !dir);
  }
  else
  {
    adjust_balance(root, dir, bal);
    root = rotate_double(root, !dir);
  }

  return root;
}

/**
 * Recursive function behind remove.
 * @param root Root element to check for being an appropriate ex-father.
 * @param key Key if the item.
 * @param done Flag to terminate recursion once it has completed.
 * @return Pointer to new root node.
 */
sc_mapitem *sc_map::remove_r(sc_mapitem *root, const char *key, int *done)
{
  if(root != NULL)
  {
    int dir;

    // Remove node
    if(root->mKey->compare(key) == 0)
    {
      // Unlink and fix parent
      if(root->mLink[0] == NULL || root->mLink[1] == NULL)
      {
        sc_mapitem *save;

        // maintain linked list
        if(root->pPrev) root->pPrev->pNext = root->pNext;
        if(root->pNext) root->pNext->pPrev = root->pPrev;

        if(root == mFirst)
          mFirst = root->pNext;
        else if(root == mLast)
          mLast = root->pPrev;

        dir  = root->mLink[0] == NULL;
        save = root->mLink[dir];
        delete root;

        mLength--;

        return save;
      }
      else
      {
        // Find inorder predecessor
        sc_mapitem *heir = root->mLink[0];

        while(heir->mLink[1] != NULL)
          heir = heir->mLink[1];

        /* Copy and set new search data */
        root->mKey = heir->mKey;
        key        = heir->mKey->get();
      }
    }

    dir = (int)(root->mKey->compare(key) > 0);

    root->mLink[dir] = remove_r(root->mLink[dir], key, done);
    if(root->mLink[dir]) root->mLink[dir]->pRoot = root;

    if(!*done)
    {
      // Update balance factors
      root->mBalance += dir != 0 ? -1 : 1;

      // Terminate or rebalance as necessary
      if(abs(root->mBalance) == 1)
        *done = 1;
      else if(abs(root->mBalance) > 1)
        root = remove_balance(root, dir, done);
    }
  }

  return root;
}

/**
 * Fixes balance after insert.
 * @param root Root element to fix balance for.
 * @param dir Direction.
 * @param done Flag to terminate recursion once it has completed.
 * @return Pointer to node that has become root
 */
inline sc_mapitem *sc_map::remove_balance(sc_mapitem *root, int dir, int *done)
{
  sc_mapitem *n = root->mLink[!dir];
  int bal       = dir == 0 ? -1 : 1;

  if(n->mBalance == -bal)
  {
    root->mBalance = n->mBalance = 0;
    root                         = rotate_single(root, dir);
  }
  else if(n->mBalance == bal)
  {
    adjust_balance(root, !dir, -bal);
    root = rotate_double(root, dir);
  }
  else
  {
    root->mBalance = -bal;
    n->mBalance    = bal;
    root           = rotate_single(root, dir);
    *done          = 1;
  }

  return root;
}

/**
 * Clears an item and all it's children.
 * @param item Root element to start from.
 */
void sc_map::clear_r(sc_mapitem *item)
{
  if(item)
  {
    if(item->mLink[0]) clear_r(item->mLink[0]);
    if(item->mLink[1]) clear_r(item->mLink[1]);
    delete item;
  }
}

/**
 * Searches the map for element with such key.
 * @param key Key to be searched for.
 * @return Pointer to found item or null.
 */
sc_mapitem *sc_map::find(const char *key)
{
  sc_mapitem *item = mRoot;
  int res, dir;
  while(item)
  {
    res = item->mKey->compare(key);
    if(!res) return item;

    dir  = (int)(res > 0);
    item = item->mLink[dir];
  }

  return NULL;
}

/**
 * Returns the length of the map.
 */
inline long sc_map::length()
{
  return mLength;
}

/**
 * Updates or inserts a value according to it's key.
 * @param key Key of the item.
 * @param val Value of the item.
 */
void sc_map::set(const char *key, const char *val)
{
  sc_mapitem *item = find(key);
  if(item)
    item->mValue->set(val);
  else
    insert(key, val);
}

/**
 * Updates or inserts a value according to it's key.
 * @param key Key of the item.
 * @param val Value of the item.
 */
inline void sc_map::set(const char *key, ic_string *val)
{
  set(key, val->get());
}

/**
 * Updates or inserts a value according to it's key.
 * @param key Key of the item.
 * @param val Value of the item.
 */
inline void sc_map::set(ic_string *key, ic_string *val)
{
  set(key->get(), val->get());
}

/**
 * Returns a value according to it's key.
 * @param key Key of the item.
 * @return val Value of the item.
 */
char *sc_map::get(const char *key)
{
  sc_mapitem *item = find(key);
  if(item)
    return item->mValue->get();
  else
    return NULL;
}

/**
 * Returns a value according to it's key.
 * @param key Key of the item.
 * @return val Value of the item.
 */
inline char *sc_map::get(ic_string *key)
{
  return get(key->get());
}

/**
 * Removes a record according to it's key.
 * @param key Key of the item.
 */
void sc_map::remove(const char *key)
{
  int done = 0;
  mRoot    = remove_r(mRoot, key, &done);
  if(mRoot) mRoot->pRoot = NULL;

  iter_rewind();
  key_rewind();
}

/**
 * Removes a record according to it's key.
 * @param key Key of the item.
 */
inline void sc_map::remove(ic_string *key)
{
  remove(key->get());
}

/**
 * Removes all records.
 */
void sc_map::clear()
{
  if(mRoot) clear_r(mRoot);

  mLength = 0;
  mCurr = mCurrKey = mFirst = mLast = mRoot = NULL;
}

/**
 * Returns next key\item pair from the tree in time-based order.
 * @return Next key\item pair.
 */
sc_mapitem *sc_map::iter_next()
{
  sc_mapitem *curr = mCurr;
  if(!curr) return NULL;

  mCurr = mCurr->pNext;
  return curr;
}

/**
 * Rewinds the time-based iteration.
 */
void sc_map::iter_rewind()
{
  mCurr = mFirst;
}

/**
 * Returns next key\item pair from the tree in key-based order.
 * @return Next key\item pair.
 */
sc_mapitem *sc_map::key_next()
{
  sc_mapitem *curr = mCurrKey;
  if(!curr) return NULL;

  // if current point has right child, get it's leftmost subchild
  if(mCurrKey->mLink[1])
  {
    mCurrKey = mCurrKey->mLink[1];
    while(mCurrKey->mLink[0])
      mCurrKey = mCurrKey->mLink[0];
  }
  // if current point is the leaf, rise up by 1 level while walking the
  // right-most branch
  else
  {
    sc_mapitem *root;
    while(mCurrKey)
    {
      root = mCurrKey->pRoot;
      if(root)
      {
        if(root->mLink[0] == mCurrKey)
        {
          mCurrKey = root;
          break;
        }
      }

      mCurrKey = root;
    }
  }

  return curr;
}

/**
 * Rewinds the key-based iteration.
 */
inline void sc_map::key_rewind()
{
  mCurrKey = mRoot;
  while(mCurrKey && mCurrKey->mLink[0])
    mCurrKey = mCurrKey->mLink[0];
}

/**
 * Returns an array containing all keys from the map as char pointers.
 */
sc_voidarray *sc_map::get_keys()
{
  sc_voidlist list;
  iter_rewind();
  sc_mapitem *curr;
  while(curr = iter_next())
    list.add(curr->mKey);
  return list.pack();
}

/**
 * Indexing operator.
 * @param key Key of the item
 * @return Link to value of the item.
 */
ic_string &sc_map::operator[](const char *key)
{
  sc_mapitem *item = find(key);
  if(!item) item = insert(key, "");
  return *(item->mValue);
}

/**
 * Indexing operator.
 * @param key Key of the item
 * @return Link to value of the item.
 */
inline ic_string &sc_map::operator[](ic_string &key)
{
  sc_mapitem *item = find(key.get());
  if(!item) item = insert(key.get(), "");
  return *(item->mValue);
}

/**
 * Indexing operator.
 * @param key Key of the item
 * @return Link to value of the item.
 */
inline ic_string &sc_map::operator[](ic_string *key)
{
  sc_mapitem *item = find(key->get());
  if(!item) item = insert(key->get(), "");
  return *(item->mValue);
}

#if MALCO_DEBUG == 1
/**
 * Displays debug info about the item.
 * @param item Item to be debugged.
 */
void sc_map::debug(sc_mapitem *item)
{
  if(item == NULL) item = mRoot;
  printf("Item @ %p: ", item);
  printf("Key: %s; Value: %s\n", item->mKey ? item->mKey->get() : "", item->mValue ? item->mValue->get() : "");
  printf("Left: [%p], Right: [%p], Root: [%p]\n-----\n", item->mLink[0], item->mLink[1], item->pRoot);

  if(item->mLink[0]) debug(item->mLink[0]);
  if(item->mLink[1]) debug(item->mLink[1]);
}
#endif

#endif
