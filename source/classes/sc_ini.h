/**
 * @file sc_ini.h
 * @author ForNeVeR.
 * sc_ini header.
 * Defines properties and methods of sc_ini class.
 */

#ifndef SC_INI_H
#define SC_INI_H

/**
 * sc_ini_section constructor.
 * @param name Section name.
 */
sc_ini_section::sc_ini_section(const char *name)
{
  mName = new char[std::strlen(name) + 1];
  if(!mName)
    ERROR(M_ERR_NO_MEMORY, M_EMODE_ERROR);
  std::strcpy(mName, name);
}

/**
 * sc_ini_section destructor.
 */
sc_ini_section::~sc_ini_section()
{
  delete[] mName;
}

/**
 * sc_ini constructor.
 * Constructs empty instance of class.
 */
sc_ini::sc_ini()
{
  section_count = 0;
  last_section_name = new char[1];
  if(!last_section_name)
    ERROR(M_ERR_NO_MEMORY, M_EMODE_ERROR);
  last_section_name[0] = '\0';
  mFilename = NULL;
}

/**
 * sc_ini constructor.
 * Constructs instance of class and loads contents from file.
 * @param filename Name of loading file.
 */
sc_ini::sc_ini(const char *filename)
{
  section_count = 0;
  last_section_name = NULL;
  // We don't need to fill last_section_name here because file_load will do it.
  mFilename = NULL;
  file_load(filename);
}

/**
 * sc_ini destructor.
 */
sc_ini::~sc_ini()
{
  for(long i = 0; i < sections.length(); i++)
    delete (sc_ini_section *) sections[i];

  delete [] last_section_name;
  delete [] mFilename;
}

/**
 * Clears current contents of object and loads new contents from file.
 * @param filename Name of loading file.
 */
void sc_ini::file_load(const char *filename)
{
  // clean contents of previous ini and re-init some fields
  for(long i = 0; i < sections.length(); i++)
  {
    delete (sc_ini_section *) sections[i];
    sections.del(i);
  }

  delete[] last_section_name;
  last_section_name = new char[1];
  if(!last_section_name)
    ERROR(M_ERR_NO_MEMORY, M_EMODE_ERROR);
  last_section_name[0] = '\0';

  if(mFilename)
  {
    delete [] mFilename;
    mFilename = NULL;
  }

  mFilename = new char[strlen(filename) + 1];
  if(!mFilename)
    ERROR(M_ERR_NO_MEMORY, M_EMODE_ERROR);
  strcpy(mFilename, filename);

  // fill this object with new contents
  ic_string content;
  content.file_load(mFilename);
  long pos = 0; // position in file

  while(pos < content.length())
  {
    ic_string *line = content.substr_get(pos, content.substr_first("\n", pos) - pos + 1);
    if(line->length() == 0) // happens on last line in file
    {
      delete line;
      line = content.substr_get(-(long)(content.length() - pos), 0);
    }
    pos += line->length();

    line->trim();
    if(line->length() != 0)
    {
      switch(line->char_at(0))
      {
      case ';': // comment
        break;
      case '[':
        if(line->char_at(line->length() - 1) == ']')
        // not properly-named value or section else; ignoring
        {
          ic_string *old_line = line;
          line = old_line->substr_get(1, old_line->length() - 2);
          delete old_line;
          line->trim(); // here we got name of section in line

          add_section(line->get());
        }
        break;
      default: // value name
        {
          long equal_pos = line->substr_first("="); // position of = sign
          if(equal_pos > 0)
          // we also ignore lines starting with '=' or that don't have '=' at all
          {
            ic_string *param_name = line->substr_get(0, equal_pos);
            param_name->rtrim();
            ic_string *value = line->substr_get(equal_pos + 1);
            value->ltrim();

            set_value(param_name->get(), value->get()); // adds parameter into last opened section

            delete param_name;
            delete value;
          }
        }
      }
    }

    delete line;
  }
}

/**
 * Saves ini contents to file, replacing an existing one.
 * @param filename Name of replacing file.
 */
void sc_ini::file_save(const char *filename)
{
  if(filename == NULL)
    filename = mFilename;

  // TODO: Exception if mFilename == NULL
  ic_string file_content;
  for(long i = 0; i < section_count; i++)
  {
    file_content << "[" << ((sc_ini_section *)sections[i])->mName << "]\n";
    sc_voidarray *param_names = ((sc_ini_section *)sections[i])->params.get_keys();
    for(long j = 0; j < param_names->length(); j++)
    {
      file_content << (char *)param_names->mPtr[j] << "="
        << ((sc_ini_section *)sections[i])->params[(char *)param_names->mPtr[j]] << "\n";
    }
    delete param_names;
  }

  file_content.file_save(mFilename);
}

/**
 * Returns sc_voidarray that contains section names collection as char *.
 */
sc_voidarray *sc_ini::get_sections()
{
  sc_voidarray *result = new sc_voidarray(section_count);
  for(long i = 0; i < section_count; i++)
  {
    char *section_name = new char[std::strlen(((sc_ini_section *)sections[i])->mName) + 1];
    if(!section_name)
      ERROR(M_ERR_NO_MEMORY, M_EMODE_ERROR);
    std::strcpy(section_name, ((sc_ini_section *)sections[i])->mName);
    result->add(section_name);
  }
  return result;
}


/**
 * Returns sc_voidarray that contains section parameter names collection as char *.
 * @param section_name Name of section, parameter names of which you want to extract.
 */
sc_voidarray *sc_ini::get_params(const char *section_name)
{
  int section_index = find_section(section_name);
  if(section_index != -1)
  {
    return ((sc_ini_section *)sections[section_index])->params.get_keys();
  }

  return new sc_voidarray();
}

/**
 * Returns parameter value from section or default value if parameter doesn't exists.
 * @param param_name Name of parameter.
 * @param section_name Name of section.
 * @param default_value Default value of parameter (will be returned if parameter doesn't exists).
 */
ic_string *sc_ini::get_value(const char *param_name, const char *section_name, const char *default_value)
{
  long section_index = find_section(section_name);
  if(section_index != -1)
  {
    return new ic_string(((sc_ini_section *)sections[section_index])->params[param_name]);
  }
  return new ic_string(default_value);
}

/**
 * Sets parameter value.
 * @note Changes do not placed in file by this method; you need to call file_save() for it.
 * @param param_name Name of parameter.
 * @param value New value of parameter.
 * @param section_name Name of section.
 */
void sc_ini::set_value(const char *param_name, const char *value, const char *section_name)
{
  sc_ini_section *section = add_section(section_name);
  section->params.set(param_name, value);
}

/**
 * Deletes parameter from ini.
 * @param param_name Name of parameter.
 * @param section_name Name of section.
 */
void sc_ini::del_param(const char *param_name, const char *section_name)
{
  long section_index = find_section(section_name);
  if(section_index != -1)
  {
    ((sc_ini_section *)sections[section_index])->params.remove(param_name);
  }

  if(section_name)
  {
    if(last_section_name)
      delete[] last_section_name;
    last_section_name = new char[strlen(section_name) + 1];
    if(!last_section_name)
      ERROR(M_ERR_NO_MEMORY, M_EMODE_ERROR);
    strcpy(last_section_name, section_name);
  }
}

/**
 * Deletes section and all its parameters.
 * @param section_name Name of section to delete.
 */
void sc_ini::del_section(const char *section_name)
{
  long section_index = find_section(section_name);
  if(section_index != -1)
    sections.del(section_index);
}

// Next methods are private, so don't need to document them.

// Get number of section in array (sections) named (section_name).
long sc_ini::find_section(const char *section_name)
{
  if(section_name)
  {
    delete[] last_section_name;
    last_section_name = new char[std::strlen(section_name) + 1];
    std::strcpy(last_section_name, section_name);
  }

  for(long i = 0; i < section_count; i++)
  {
    if(std::strcmp(((sc_ini_section *)sections[i])->mName, last_section_name) == 0)
      return i;
  }
  return -1;
}

// Add new section to array and return pointer to it or return pointer to
// already existing section named (section_name).
sc_ini_section *sc_ini::add_section(const char *section_name)
{
  long section_index = find_section(section_name);
  if(section_index != -1)
    return (sc_ini_section *)sections[section_index];

  sections.add(new sc_ini_section(section_name));
  return (sc_ini_section *)sections[section_count++];
}

#endif // SC_INI_H
