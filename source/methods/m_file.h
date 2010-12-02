/**
 * @file m_file.h
 * @author impworks
 * File method header.
 * Defines all methods for file class.
 */

#ifndef M_FILE_H
#define M_FILE_H

/**
 * File constructor.
 */
void file_op_create(rc_head *head)
{
  ic_object *name = head->rIN.pop();
  head->pCurrObj->mData = new ic_file(name);
}

/**
 * Opens the selected file.
 */
void file_open(rc_head *head)
{
  ic_object *obj = head->rIN.pop();
  if(obj->class_id() == M_CLASS_INT)
  {
    char mode = (char)((ic_int *)obj->mData)->mValue;
    bool result = ((ic_file *)head->pCurrObj->mData)->open(mode);
    head->rOUT.push(head->obj_bool(result));
  }
}

/**
 * Closes the selected file.
 */
void file_close(rc_head *head)
{
  ((ic_file *)head->pCurrObj->mData)->close();  
}

/**
 * Reads a specified amount of data from a file.
 */
void file_read(rc_head *head)
{
  ic_file *file = (ic_file *)head->pCurrObj->mData;
  if(file->mMode & IO_READ)
  {
    ic_object *obj = head->rIN.pop();
    if(obj->class_id() == M_CLASS_INT)
    {
      ic_string *str = file->read(((ic_int *)obj->mData)->mValue);
      head->rOUT.push(head->obj_string(str));
    } 
  }
  else
    head->rOUT.push(head->obj_bool(false));
}

/**
 * Reads the whole file as a string.
 */
void file_read_all(rc_head *head)
{
  ic_file *file = (ic_file *)head->pCurrObj->mData;
  if(file->mMode & IO_READ)
  {
    ic_string *str = new ic_string();
    str->file_load(((ic_file *)head->pCurrObj->mData)->mName);
    head->rOUT.push(head->obj_string(str));
  }
  else
    head->rOUT.push(head->obj_bool(false));
}

/**
 * Reads the whole file as an array of lines.
 */
void file_read_lines(rc_head *head)
{
  ic_file *file = (ic_file *)head->pCurrObj->mData;
  if(file->mMode & IO_READ)
  {
    // make array and string
    ic_string *str = new ic_string();
    ic_array *arr = new ic_array();

    long tmp_pos = ftell(file->mFile);
    fseek(file->mFile, 0, SEEK_SET);

    char buf;
    while(!feof(file->mFile))
    {
      fread(&buf, 1, 1, file->mFile);
      if(buf != '\n')
        str->append(&buf, 1);
      else
      {
        arr->append(head->obj_string(str));
        str = new ic_string();
      }
    }
    fseek(file->mFile, tmp_pos, SEEK_SET);

    arr->append(head->obj_string(str));
    head->rOUT.push(head->obj_array(arr));
  }
  else
    head->rOUT.push(head->obj_bool(false));
}

/**
 * Seeks a position inside the file.
 */
void file_seek(rc_head *head)
{
  ic_object *mode = head->rIN.pop(), *pos = head->rIN.pop();
  if((mode->class_id() & pos->class_id()) == M_CLASS_INT)
  {
    long imode = ((ic_int*)mode->mData)->mValue;
    long ipos = ((ic_int*)mode->mData)->mValue;
    ((ic_file *)head->pCurrObj->mData)->seek(ipos, imode);
  }
}

/**
 * Tells the current position in the file.
 */
void file_tell(rc_head *head)
{
  long pos = ((ic_file *)head->pCurrObj->mData)->tell();
  head->rOUT.push(head->obj_int(pos));
}

/**
 * Checks for file existence.
 */
void file_exists(rc_head *head)
{
  bool exists = sc_file::exists((ic_file)head->pCurrObj->mData)->mName);
  head->rOUT.push(head->obj_bool(exists));
}

/**
 * Returns file length.
 */
void file_length(rc_head *head)
{
  long length = ((ic_file *)head->pCurrObj->mData)->length();
  head->rOUT.push(head->obj_int(length));
}

/**
 * Copies a file to a new location.
 */
void file_copy(rc_head *head)
{
  ic_object *obj = head->rIN.pop();
  if(obj->class_id() == M_CLASS_STRING)
  {
    ic_string *name = (ic_string*)obj->mData;
    ((ic_file*)head->pCurrObj->mData)->copy(name);
  }
}

/**
 * Renames / moves the file.
 */
void file_rename(rc_head *head)
{
  ic_object *obj = head->rIN.pop();
  if(obj->class_id() == M_CLASS_STRING)
  {
    ic_string *name = (ic_string*)obj->mData;
    ((ic_file*)head->pCurrObj->mData)->rename(name);
  }
}

/**
 * Removes the file.
 */
void file_rename(rc_head *head)
{
  ((ic_file*)head->pCurrObj->mData)->remove(name);
}

/**
 * Truncates the file.
 */
void file_truncate(rc_head *head)
{
  ic_object *obj = head->rIN.pop();
  if(obj->class_id() == M_CLASS_INT)
  {
    long size = ((ic_int *)obj->mData)->mValue;
    ((ic_file*)head->pCurrObj->mData)->truncate(name);
  }
}

/**
 * Returns a boolean representation of a file.
 */
void file_to_b(rc_head *head)
{
  file_exists(head);
}

/**
 * Returns an integer representation of a file.
 */
void file_to_i(rc_head *head)
{
  file_length(head);
}

/**
 * Inspect object
 */
void file_inspect(rc_head *head)
{
  ic_string *str = new ic_string("{file:");
  ic_file *file = (ic_file *)head->pCurrObj->mData;
  str->append(file->mName);

  if(file->mFile)
  {
    ic_int *mode = new ic_int(file->mMode);
    str->append(" open(");
    str->append(mode->to_s());
    str->append(")");
  }

  str->append("}");

  head->rOUT.push(head->obj_string(str));
}

#endif