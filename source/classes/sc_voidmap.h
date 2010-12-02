/**
 * @file sc_voidmap.h
 * @author impworks.
 * sc_voidmap and sc_voidmapitem header.
 * Defines properties and methods of sc_voidmap and sc_voidmapitem classes.
 * @todo Merge it with sc_map to remove duplication of code!
 */

#ifndef SC_VOIDMAP_H
#define SC_VOIDMAP_H

//--------------------------------
//  sc_voidmapitem
//--------------------------------

/**
 * sc_voidmapitem constructor.
 * @param key Item key.
 * @param val Item value.
 */
sc_voidmapitem::sc_voidmapitem(const char *key, void *val)
{
  mBalance = 0;
  mLink[0] = mLink[1] = pRoot = NULL;
  pNext = pPrev = NULL;

  mKey = new ic_string(key);
  if(!mKey) ERROR(M_ERR_NO_MEMORY, M_EMODE_ERROR);
  mValue = val;
}

/**
 * sc_voidmapitem destructor.
 */
sc_voidmapitem::~sc_voidmapitem()
{
  delete mKey;
}

//--------------------------------
//  sc_voidmap
//--------------------------------

/**
 * sc_voidmap constructor.
 */
sc_voidmap::sc_voidmap()
{
  mRoot = mCurr = mCurrKey = mFirst = mLast = NULL;
  mLength = 0;
}

/**
 * sc_map copy constructor.
 * @param map sc_map to copy.
 */
sc_voidmap::sc_voidmap(sc_voidmap &map)
{
  mRoot = mCurr = mCurrKey = mFirst = mLast = NULL;
  mLength = 0;

  sc_voidmapitem *item;
  map.iter_rewind();
  while(item = map.iter_next())
    set(item->mKey, item->mValue);
}

/**
 * sc_voidmap destructor.
 */
sc_voidmap::~sc_voidmap()
{
  clear();
}

/**
 * Adjusts balance of the node.
 * @param root Node to be processed.
 * @param dir Direction.
 * @param bal Balance value.
 */
inline void sc_voidmap::adjust_balance(sc_voidmapitem *root, int dir, int bal)
{
  sc_voidmapitem *n = root->mLink[dir];
  sc_voidmapitem *nn = n->mLink[!dir];

  if(nn->mBalance == 0)
    root->mBalance = n->mBalance = 0;
  else if(nn->mBalance == bal)
  {
    root->mBalance = -bal;
    n->mBalance = 0;
  }
  else
  {
    root->mBalance = 0;
    n->mBalance = bal;
  }

  nn->mBalance = 0;
}

/**
 * Performs a single rotation of the node
 * @param root Node to be rotated.
 * @param dir Direction.
 * @return Node that that has become root.
 */
inline sc_voidmapitem *sc_voidmap::rotate_single (sc_voidmapitem *root, int dir)
{
  sc_voidmapitem *save = root->mLink[!dir];

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
inline sc_voidmapitem *sc_voidmap::rotate_double (sc_voidmapitem *root, int dir)
{
  sc_voidmapitem *save = root->mLink[!dir]->mLink[dir];

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
sc_voidmapitem *sc_voidmap::insert(const char *key, void *val)
{
  int done = 0;
  sc_voidmapitem *item;
  if(!key || !val) return NULL;
  mRoot = this->insert_r(mRoot, key, val, &done, &item);
  mRoot->pRoot = NULL;

  // maintain linked list
  if(!mLength)
    mFirst = item;
  else
    mLast->pNext = item;
  item->pPrev = mLast;
  mLast = item;

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
sc_voidmapitem *sc_voidmap::insert_r(sc_voidmapitem *root, const char *key, void *val, int *done, sc_voidmapitem **item)
{
  if(root == NULL)
  {
    root = new sc_voidmapitem (key, val);
    if(!root) ERROR(M_ERR_NO_MEMORY, M_EMODE_ERROR);
    *item = root;
  }
  else
  {
    int dir = (int)(root->mKey->compare(key) < 0);

    root->mLink[dir] = insert_r(root->mLink[dir], key, val, done, item );

    if(!*done)
    {
      root->mBalance += dir == 0 ? -1 : 1;

      if(root->mBalance == 0)
        *done = 1;
      else if(abs(root->mBalance) > 1)
      {
        root = insert_balance(root, dir);
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
inline sc_voidmapitem *sc_voidmap::insert_balance(sc_voidmapitem *root, int dir)
{
  sc_voidmapitem *n = root->mLink[dir];
  int bal = dir == 0 ? -1 : 1;

  if ( n->mBalance == bal )
  {
    root->mBalance = n->mBalance = 0;
    root = rotate_single(root, !dir);
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
sc_voidmapitem *sc_voidmap::remove_r(sc_voidmapitem *root, const char *key, int *done)
{
  if(root != NULL)
  {
    int dir;

    /* Remove node */
    if(root->mKey->compare(key) == 0)
    {
      /* Unlink and fix parent */
      if (root->mLink[0] == NULL || root->mLink[1] == NULL)
      {
        sc_voidmapitem *save;

        // maintain linked list
        if(root->pPrev)
          root->pPrev->pNext = root->pNext;
        if(root->pNext)
          root->pNext->pPrev = root->pPrev;

        if(root == mFirst)
          mFirst = root->pNext;
        else if(root == mLast)
          mLast = root->pPrev;

        dir = root->mLink[0] == NULL;
        save = root->mLink[dir];
        delete root;

        mLength--;

        return save;
      }
      else
      {
        /* Find inorder predecessor */
        sc_voidmapitem *heir = root->mLink[0];

        while ( heir->mLink[1] != NULL )
          heir = heir->mLink[1];

        /* Copy and set new search data */
        root->mKey = heir->mKey;
        key = heir->mKey->get();
      }
    }

    dir = (int)(root->mKey->compare(key) < 0);
    root->mLink[dir] = remove_r(root->mLink[dir], key, done);

    if(!*done)
    {
      /* Update balance factors */
      root->mBalance += dir != 0 ? -1 : 1;

      /* Terminate or rebalance as necessary */
      if(abs(root->mBalance) == 1)
        *done = 1;
      else if (abs(root->mBalance) > 1)
        root = remove_balance (root, dir, done);
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
inline sc_voidmapitem *sc_voidmap::remove_balance(sc_voidmapitem *root, int dir, int *done)
{
  sc_voidmapitem *n = root->mLink[!dir];
  int bal = dir == 0 ? -1 : 1;

  if(n->mBalance == -bal)
  {
    root->mBalance = n->mBalance = 0;
    root = rotate_single(root, dir);
  }
  else if (n->mBalance == bal)
  {
    adjust_balance(root, !dir, -bal);
    root = rotate_double(root, dir);
  }
  else
  {
    root->mBalance = -bal;
    n->mBalance = bal;
    root = rotate_single(root, dir);
    *done = 1;
  }

  return root;
}

/**
 * Clears an item and all it's children.
 * @param item Root element to start from.
 */
void sc_voidmap::clear_r(sc_voidmapitem *item)
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
sc_voidmapitem *sc_voidmap::find(const char *key)
{
  sc_voidmapitem *item = mRoot;
  int res, dir;
  while(item)
  {
    res = item->mKey->compare(key);
    if(!res)
      return item;

    dir = (int)(res < 0);
    item = item->mLink[dir];
  }

  return NULL;
}

/**
 * Returns the length of the map.
 */
inline long sc_voidmap::length()
{
  return mLength;
}

/**
 * Updates or inserts a value according to it's key.
 * @param key Key of the item.
 * @param val Value of the item.
 */
void sc_voidmap::set(const char *key, void *val)
{
  sc_voidmapitem *item = find(key);
  if(item)
  {
    item->mValue = val;
  }
  else
    insert(key, val);
}

/**
 * Updates or inserts a value according to it's key.
 * @param key Key of the item.
 * @param val Value of the item.
 */
inline void sc_voidmap::set(ic_string *key, void *val)
{
  set(key->get(), val);
}

/**
 * Returns a value according to it's key.
 * @param key Key of the item.
 * @return val Value of the item.
 */
void *sc_voidmap::get(const char *key)
{
  sc_voidmapitem *item = find(key);
  if(item)
    return item->mValue;
  else
    return NULL;
}

/**
 * Returns a value according to it's key.
 * @param key Key of the item.
 * @return val Value of the item.
 */
inline void *sc_voidmap::get(ic_string *key)
{
  return get(key->get());
}

/**
 * Removes a record according to it's key.
 * @param key Key of the item.
 */
void sc_voidmap::remove(const char *key)
{
  int done = 0;
  mRoot = remove_r(mRoot, key, &done);
  mRoot->pRoot = NULL;
  iter_rewind();
  key_rewind();
}

/**
 * Removes a record according to it's key.
 * @param key Key of the item.
 */
inline void sc_voidmap::remove(ic_string *key)
{
  remove(key->get());
}

/**
 * Removes all records.
 */
void sc_voidmap::clear()
{
  if(mRoot)
    clear_r(mRoot);

  mLength = 0;
  mCurr = mCurrKey = mFirst = mLast = mRoot = NULL;
}

/**
* Returns next key\item pair from the tree in time-based order.
* @return Next key\item pair.
*/
sc_voidmapitem *sc_voidmap::iter_next()
{
  sc_voidmapitem *curr = mCurr;
  if(!curr) return NULL;

  mCurr = mCurr->pNext;
  return curr;
}

/**
* Rewinds the time-based iteration.
*/
void sc_voidmap::iter_rewind()
{
  mCurr = mFirst;
}

/**
 * Returns next key from the tree.
 * @return Next key.
 */
sc_voidmapitem *sc_voidmap::key_next()
{
  sc_voidmapitem *curr = mCurrKey;

  if(!curr) return NULL;

  // if current point has right child, get it's leftmost subchild
  if(mCurrKey->mLink[1])
  {
    mCurrKey = mCurrKey->mLink[1];
    while(mCurrKey->mLink[0])
      mCurrKey = mCurrKey->mLink[0];
  }
  // if current point is the leaf, rise up by 1 level while walking the right-most branch
  else
  {
    sc_voidmapitem *root;
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
 * Rewinds the iteration.
 */
inline void sc_voidmap::key_rewind()
{
  mCurrKey = mRoot;
  while(mCurrKey->mLink[0])
    mCurrKey = mCurrKey->mLink[0];
}

/**
 * Returns an array containing all keys from the map as (ic_string *).
 */
sc_voidarray *sc_voidmap::get_keys()
{
  sc_voidlist list;
  iter_rewind();
  sc_voidmapitem *curr;
  while(curr = iter_next())
    list.add(curr->mKey);
  return list.pack();
}

/**
 * Indexing operator.
 * @param key Key of the item
 * @return Link to value of the item.
 */
inline void *sc_voidmap::operator[](const char *key)
{
  sc_voidmapitem *item = find(key);
  if(!item) item = insert(key, NULL);
  return item->mValue;
}

/**
 * Indexing operator.
 * @param key Key of the item
 * @return Link to value of the item.
 */
inline void *sc_voidmap::operator[](ic_string &key)
{
  sc_voidmapitem *item = find(key.get());
  if(!item) item = insert(key.get(), NULL);
  return item->mValue;
}

/**
 * Indexing operator.
 * @param key Key of the item
 * @return Link to value of the item.
 */
inline void *sc_voidmap::operator[](ic_string *key)
{
  sc_voidmapitem *item = find(key->get());
  if(!item) item = insert(key->get(), NULL);
  return item->mValue;
}

#if MALCO_DEBUG == 1
/**
 * Displays debug info about the item.
 * @param item Item to be debugged.
 */
void sc_voidmap::debug(sc_voidmapitem *item)
{
  if(item == NULL) item = mRoot;
  printf("Item @ %p\n", item);
  printf("Key: %s\nValue: %p\n", item->mKey ? item->mKey->get() : "", item->mValue);
  printf("Left: [%p], Right: [%p]\n-----\n", item->mLink[0], item->mLink[1]);

  if(item->mLink[0]) debug(item->mLink[0]);
  if(item->mLink[1]) debug(item->mLink[1]);
}
#endif

#endif
