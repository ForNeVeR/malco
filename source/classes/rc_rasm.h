/**
 * @file rc_rasm.h
 * @author ForNeVeR.
 * rc_rasm header.
 * Defines methods of rc_rasm class.
 */

#ifndef RC_RASM_H
#define RC_RASM_H

// TODO: Add static command parameters checking.

/**
 * rc_rasm constructor, initializes class instalce and loads code into internal
 * storage.
 * @param core Pointer to rc_core class instance.
 * @param code Pointer to ic_string object from where code will be copied.
 * @param filename Name of file code from where was loaded. Using for
 * exceptions' output. Should not be NULL.
 */
rc_rasm::rc_rasm(rc_core *core, const ic_string *code, const char *filename)
  : pCore(core), mTape(), mDefTable(core), mStrTable(), mCode(*code),
  mFileName(filename)
{
  mLineNumber = 0;
  mCurrentMethodName = NULL;
}

/**
 * rc_rasm destructor.
 */
rc_rasm::~rc_rasm()
{
  // Clear variables:
  for(int i = 0; i < mVariables.length(); ++i)
  {
    delete[] (char *)mVariables[i];
  }

  // Clear class name stack:
  for (int i = 0; i < mClassNameStack.length(); ++i)
  {
    delete[] (char *)mClassNameStack[i];
  }

  if(mCurrentMethodName)
    delete mCurrentMethodName;
}

/**
 * Parses code, assembles it and saves all data to specified path. Example: if
 * path == "C:\Temp\foo", then next files will be produced: C:\Temp\foo.rbc for
 * bytecode, foo.rdt for definition table and foo.rst for string table.
 */
void rc_rasm::assemble(const ic_string *path)
{
  sc_voidarray *lines = mCode.split("\n");
  int lineIndex = 0;
  int command_counter = 0;
  
  // Label processing loop:
  for(lineIndex = 0; lineIndex < lines->length(); ++lineIndex)
  {
    ic_string *line = (ic_string *)lines->mPtr[lineIndex];
    ic_string *cleaned_line = clean_line(line);
    if(*cleaned_line != "")
    {
      if(is_command(cleaned_line))
      {
        ++command_counter;
      }
      else if(is_label(cleaned_line))
      {
        ic_string *label_name = extract_string_parameter(line);
        mLabelMap.set(label_name, (void *)(command_counter));
        delete label_name;
      }
    }
    delete cleaned_line;
  }

  // Assembling loop:
  try
  {
    for(lineIndex = 0; lineIndex < lines->length(); ++lineIndex)
    {
      ic_string *line = (ic_string *)lines->mPtr[lineIndex];
      ic_string *cleaned_line = clean_line(line);
      if(*cleaned_line != "")
      {
        process_line(cleaned_line);
      }
      delete cleaned_line;
    }
  }
  catch(sc_exception *)
  {
    // Set data for error condition:
    mLineNumber = lineIndex + 1;
    // Clear lines array:
    for (int i = 0; i < lines->length(); ++i)
      delete (ic_string *)lines->mPtr[i];

    delete lines;
    // Re-throw the exception:
    throw;
  }

  // Clear lines array:
  for (int i = 0; i < lines->length(); ++i)
    delete (ic_string *)lines->mPtr[i];

  delete lines;

  // TODO: Save all files.
  mTape.file_save("test.rbc");
  mStrTable.file_save("test.rst");
  mDefTable.file_save("test.rdt");
}

/**
 * @returns Number of line caused error.
 */
int rc_rasm::get_error_line_number() const
{
  return mLineNumber;
}

/**
 * @returns Name of file caused error.
 */
ic_string *rc_rasm::get_error_filename()
{
  return &mFileName;
}

/**
 * Assembles single line of code. Updates all internal objects if necessary.
 */
void rc_rasm::process_line(ic_string *line)
{
  ic_string *command_name = extract_command_name(line);
  
  // Pseudo commands:
  if(*command_name == "LABEL")
  {
    // Nothing to do here.
  }
  else if(*command_name == "FUNC")
  {
    ic_string *func_name = extract_string_parameter(line);

    if(mCurrentMethodName)
    {
      ic_string *current_class = get_current_class();
      ic_string *error_msg = ic_string::format(M_ERR_RASM_NESTED_METHOD,
        func_name->get(), current_class->get());
      
      delete command_name;
      delete func_name;
      delete current_class;
      ERROR(error_msg, M_EMODE_COMPILE);
    }
    mCurrentMethodName = func_name;

    short modifiers = extract_modifiers(line);
    int min_args = extract_int_parameter(line, 1);
    bool splat = !has_parameter(line, 2);
    int max_args = splat ? min_args : extract_int_parameter(line, 2);
    
    if((modifiers & !M_PROP_FINAL & !M_PROP_STATIC & !M_PROP_PRIVATE &
      !M_PROP_PUBLIC) != 0)
    {
      ic_string *error_msg = ic_string::format(M_ERR_RASM_BAD_MOD,
        func_name->get());
      
      delete command_name;
      ERROR(error_msg, M_EMODE_COMPILE);
    }

    ic_string *current_class = get_current_class();

    mDefTable.add_method(func_name->get(), current_class->get(), modifiers,
      mTape.length(), min_args, max_args, splat, &sc_voidarray());

    delete current_class;
  }
  else if(*command_name == "CLASS")
  {
    short modifiers = extract_modifiers(line);
    ic_string *class_name = extract_string_parameter(line);

    if((modifiers & !M_PROP_FINAL & !M_PROP_STUB) != 0)
    {
      ic_string *error_msg = ic_string::format(M_ERR_RASM_BAD_MOD,
        class_name->get());
      
      delete command_name;
      ERROR(error_msg, M_EMODE_COMPILE);
    }

    ic_string *parent_name = NULL;
    if(has_parameter(line, 1))
    {
      parent_name = extract_string_parameter(line, 1);
    }
    else
    {
      parent_name = new ic_string("", 0);
    }

    ic_string *current_class = get_current_class();
    mDefTable.add_class(class_name->get(), parent_name->get(), modifiers,
      current_class->get());
    delete current_class;
    
    char *class_name_cstr = new char[class_name->length() + 1];
    std::strcpy(class_name_cstr, class_name->get());
    mClassNameStack.add(class_name_cstr);

    delete parent_name;
    delete class_name;
  }
  else if(*command_name == "VAR")
  {
    short modifiers = extract_modifiers(line);
    ic_string *var_name = extract_string_parameter(line);

    if((modifiers & !M_PROP_FINAL & !M_PROP_STATIC & !M_PROP_PRIVATE &
      !M_PROP_PUBLIC) != 0)
    {
      ic_string *error_msg = ic_string::format(M_ERR_RASM_BAD_MOD,
        var_name->get());
      
      delete command_name;
      ERROR(error_msg, M_EMODE_COMPILE);
    }

    ic_string *current_class = get_current_class();
    mDefTable.add_property(*var_name, current_class->get(), modifiers);

    delete current_class;
    delete var_name;
  }
  else if(*command_name == "END")
  {
    // If any method opened, this command closes method definition:
    if(mCurrentMethodName)
    {
      delete mCurrentMethodName;
      mCurrentMethodName = NULL;
      // Clear local variables array:
      int length = mVariables.length();
      for(int i = 0; i < length; ++i)
      {
        delete[] (char *)mVariables[0];
        mVariables.del(0);
      }      
    }
    else
    {
      // END pseudo-command closes class declaration, so last class name have
      // to be deleted from stack:
      long last_index = mClassNameStack.length() - 1;
      delete[] (char *)mClassNameStack[last_index];
      mClassNameStack.del(last_index);
    }
  }
  else // non-pseudo commands
  {
    rc_cmd cmd;
    cmd.mCmd = get_command_code(command_name);

    if(cmd.mCmd == RASM_CMD_WTF)
    {
      ic_string *msg = ic_string::format(M_ERR_RASM_UNKNOWN_CMD,
        command_name->get());
      
      delete command_name;
      ERROR(msg, M_EMODE_COMPILE);
    }

    cmd.mModifier = 0;
    cmd.mParam.addr = 0;

    // Label name processing:
    if(cmd.mCmd == RASM_CMD_JMP || cmd.mCmd == RASM_CMD_JTRUE ||
      cmd.mCmd == RASM_CMD_JFALSE || cmd.mCmd == RASM_CMD_TRY)
    {
      ic_string *label_name = extract_string_parameter(line);
      if(mLabelMap.find(label_name->get()))
      {
        cmd.mParam.addr = (int)mLabelMap[label_name];
      }
      else
      {
        ic_string *msg = ic_string::format(M_ERR_RASM_BAD_LABEL_NAME,
          label_name->get());
        
        delete command_name;
        delete label_name;
        ERROR(msg, M_EMODE_COMPILE);
      }
    }
    else if(has_parameter(line))
    {
      // Argument processing:
      ic_string *arg = extract_raw_parameter(line);
      if(*arg == "NULL")
      {
        cmd.mModifier = RASM_MOD_NULL;
      }
      else if(*arg == "SELF")
      {
        cmd.mModifier = RASM_MOD_SELF;
      }
      else if(*arg == "TRUE")
      {
        cmd.mModifier = RASM_MOD_TRUE;
      }
      else if(*arg == "FALSE")
      {
        cmd.mModifier = RASM_MOD_FALSE;
      }
      else if(*arg == "UNDEF")
      {
        cmd.mModifier = RASM_MOD_UNDEF;
      }
      else if(*arg == "PROPERTY")
      {
        cmd.mModifier = RASM_MOD_PROPERTY;
        ic_string *property_name = extract_string_parameter(line, 1);
        cmd.mParam.addr = mStrTable.add(property_name);
        delete property_name;
      }
      else if(*arg == "VAR")
      {
        cmd.mModifier = RASM_MOD_VAR;
        ic_string *variable_name = extract_string_parameter(line, 1);
        cmd.mParam.addr = find_variable(variable_name);
        delete variable_name;
      }
      else if(*arg == "CONST")
      {
        cmd.mModifier = RASM_MOD_CONST;
        ic_string *const_arg = extract_string_parameter(line, 1);
        cmd.mParam.addr = mStrTable.add(const_arg);
        delete const_arg;
      }
      else if(is_parameter_int(line))
      {
        cmd.mModifier = RASM_MOD_INT;
        cmd.mParam.addr = extract_int_parameter(line);
      }
      else if(is_parameter_float(line))
      {
        cmd.mModifier = RASM_MOD_FLOAT;
        cmd.mParam.val = extract_float_parameter(line);
      }
      else if(is_parameter_string(line))
      {
        cmd.mModifier = RASM_MOD_STRING;
        ic_string *string_arg = extract_string_parameter(line);
        cmd.mParam.addr = mStrTable.add(string_arg);
        delete string_arg;
      }
      else
      {
        ic_string *msg = ic_string::format(M_ERR_RASM_BAD_LINE, line->get());
        
        delete command_name;
        ERROR(msg, M_EMODE_COMPILE);
      }
    }

    mTape.add(&cmd);
  }

  delete command_name;
}

/**
 * @returns Last class name in class stack.
 */
inline ic_string *rc_rasm::get_current_class()
{
  if(mClassNameStack.length() == 0)
  {
    return new ic_string("object");
  }
  else
  {
    ic_string *class_name = new ic_string();
    for(int i = 0; i < mClassNameStack.length(); ++i)
    {
      if(i != 0)
        class_name->append("::", 2);
      class_name->append((char *)mClassNameStack[i]);
    }
    return class_name;
  }
}

/**
 * If necessary, adds variable name to local variables table. Returns index
 * from that table.
 */
long rc_rasm::find_variable(ic_string *name)
{
  for(long i = 0; i < mVariables.length(); ++i)
  {
    char *variable_name = (char *)mVariables[i];
    if (name->compare(variable_name) == 0)
      return i;
  }

  char *new_variable_name = new char[name->length() + 1];
  std::strcpy(new_variable_name, name->get());
  mVariables.add(new_variable_name);
  return mVariables.length() - 1;
}

/**
 * Trims all leading whitespace and trailing whitespace and comments.
 * @param line Line to clean.
 * @returns A cleaned line.
 */
ic_string *rc_rasm::clean_line(ic_string *line)
{
  // In python syntax this regex was
  // r'\s*((?:(?:.*?)(?:".*?(?<!\\)(?:\\\\)*")?)+)\s*(?://.*)?$'.
  ic_regex regex = ic_regex("/^\\s*"     // leading whitespace
    "((?:(?:.*?)"                        // just some text
    "(?:\".*?(?<!\\\\)(?:\\\\\\\\)*\")?" // possibly string literal
    ")+)"                                // multiple of "text+string" patterns
    "\\s*(?://.*)?$/"                    // trailing whitespace and a comment
    );

  ic_match *match = regex.match(line);
  ic_string *cleaned_line = new ic_string(match->get(1));
  delete match;

  return cleaned_line;
}

// Macros for generating command codes:
#define COMMAND_FIRST(cmdname)        \
  if(command->compare(#cmdname) == 0) \
  {                                   \
    return RASM_CMD_##cmdname;        \
  }
#define COMMAND(cmdname) else COMMAND_FIRST(cmdname)

/**
 * @param command Line containing ONLY command name (returned from parse_command_name).
 * @returns Code of command or RASM_CMD_WTF if no such command exists.
 */
int rc_rasm::get_command_code(ic_string *command)
{
  // data transfer
  COMMAND_FIRST(LOADAX)
  COMMAND(LOADBX)
  COMMAND(SAVEAX)     
  COMMAND(SAVEBX)     
  COMMAND(XCHG)       
  COMMAND(ASSIGN)     
  COMMAND(UNSPLASSIGN)

  // basic operations
  COMMAND(ADD)
  COMMAND(SUB)
  COMMAND(MUL)
  COMMAND(DIV)
  COMMAND(MOD)
  COMMAND(POW)
  COMMAND(SHL)
  COMMAND(SHR)
  COMMAND(BAND)
  COMMAND(BOR)
  COMMAND(BXOR)
  COMMAND(AND)
  COMMAND(OR)
  COMMAND(XOR)

  // unary operations
  COMMAND(INC)
  COMMAND(DEC)
  COMMAND(NEG)

  // comparison and jumps
  COMMAND(EQ)
  COMMAND(EQ_STRICT)
  COMMAND(REL)
  COMMAND(LESS)
  COMMAND(LESS_EQ)
  COMMAND(GREATER)
  COMMAND(GREATER_EQ)
  COMMAND(CMP)
  COMMAND(JTRUE)
  COMMAND(JFALSE)
  COMMAND(JMP)

  // stack commands
  COMMAND(PUSHUS)
  COMMAND(POPUS)
  COMMAND(PUSHSRC)
  COMMAND(POPSRC)
  COMMAND(PUSHDST)
  COMMAND(POPDST)
  COMMAND(SPLAT)
  COMMAND(UNSPLAT)
  COMMAND(CLRSRC)
  COMMAND(CLRDST)

  // object manipulation
  COMMAND(NEW)
  COMMAND(CALL)
  COMMAND(RETURN)
  COMMAND(NSP)
  COMMAND(BINDLAMBDA)

  // array-specific actions
  COMMAND(INDEX)

  // interpreter behaviour modification
  COMMAND(INCLUDE)
  COMMAND(REQUIRE)
  COMMAND(GC)
  COMMAND(SETPTY)
  COMMAND(SETFILE)
  COMMAND(SETLINE)
  COMMAND(THROW)
  COMMAND(TRY)
  COMMAND(TRIED)
  COMMAND(EXIT)

  // class table generator commands
  COMMAND(REGCLASS)
  COMMAND(REGPROPERTY)
  COMMAND(REGMETHOD)

  COMMAND(INSPECT)

  return RASM_CMD_WTF;
}

#undef COMMAND_FIRST
#undef COMMAND

/**
 * @returns Is line contains command call or not.
 */
bool rc_rasm::is_command(ic_string *line)
{
  ic_string *command_name = extract_command_name(line);
  bool contains_command = get_command_code(command_name) != RASM_CMD_WTF;
  delete command_name;
  return contains_command;
}

/**
 * @returns Is line contains LABEL pseudo-command or not.
 */
bool rc_rasm::is_label(ic_string *line)
{
  ic_string *command_name = extract_command_name(line);
  bool contains_label = *command_name == "LABEL";
  delete command_name;
  return contains_label;
}

/**
 * Splits line into words and quoted arguments.
 * @returns sc_voidarray with words as (ic_string *).
 */
sc_voidarray rc_rasm::split_into_primitives(ic_string *line)
{
  // And again, regular expression. In python it was
  // r'((?:".*?(?<!\\)(?:\\\\)*")|(?:\S+))'.
  ic_regex regex = ic_regex("/((?:\".*?(?<!\\\\)(?:\\\\\\\\)*\")|(?:\\S+))/");
  // This regex means any quoted parameters (escaping counts!) or single-word
  // parameters.

  sc_voidarray result;

  ic_match *match = regex.match(line);
  while(match && match->count() > 0)
  {
    long bounds[2] = { 0 };
    match->bounds(1, bounds);
    long pos = bounds[1] + 1;

    ic_string *parameter = new ic_string(match->get(1));
    result.add(parameter);

    delete match;
    match = regex.match(line, pos);
  }

  delete match;

  return result;
}

/**
 * @returns Pointer to new ic_string object containing command name extracted
 * from the line.
 */
ic_string *rc_rasm::extract_command_name(ic_string *line)
{
  long i = 0; // first whitespace character index
  while(line->length() > i &&
    !std::strchr(ic_string::whitespace, line->char_at(i)))
  {
    ++i;
  }

  return line->substr_get(0, i);
}

/**
 * @returns short value representing all private / public modifiers used in
 * definition.
 */
short rc_rasm::extract_modifiers(ic_string *line)
{
  short modifiers = 0;
  sc_voidarray *words = line->split(&ic_regex("/\\s+/"));
  
  // First word is command name, so we start parsing from second.
  for(int i = 1; i < words->length(); ++i)
  {
    ic_string *word = (ic_string *)words->mPtr[i];
    short current_modifier = 0;
    if(*word == "final")
      current_modifier ^= M_PROP_FINAL;
    else if(*word == "stub")
      current_modifier ^= M_PROP_STUB;
    else if(*word == "static")
      current_modifier ^= M_PROP_STATIC;
    else if(*word == "private")
      current_modifier ^= M_PROP_PRIVATE;
    else if(*word == "public")
      current_modifier ^= M_PROP_PUBLIC;
    else
    {
      delete word;
      break;
    }

    delete word;

    if(!(modifiers & current_modifier))
      modifiers ^= current_modifier;
    else
      ERROR(M_ERR_RASM_WRONG_MOD_SET, M_EMODE_COMPILE);
  }

  delete words;
  return modifiers;
}

/**
 * @returns true if line has parameter with index.
 */
bool rc_rasm::has_parameter(ic_string *line, int index)
{
  ic_string *parameter = (ic_string *)extract_raw_parameter(line, index);
  if (parameter != NULL)
  {
    delete parameter;
    return true;
  }
  else
    return false;
}

/**
 * @returns Raw parameter text.
 */
ic_string *rc_rasm::extract_raw_parameter(ic_string *line, int index)
{
  sc_voidarray words = split_into_primitives(line);
  ic_string *parameter = NULL;
  int first_parameter_index = 1;

  // Find first real parameter in line:
  for(int i = first_parameter_index; i < words.length(); ++i)
  {
    ic_string *maybe_parameter = (ic_string *)words[i];
    if(*maybe_parameter == "final" ||*maybe_parameter == "stub" ||
      *maybe_parameter == "static" ||*maybe_parameter == "private" ||
      *maybe_parameter == "public")
    {
      ++first_parameter_index;
    }
    else
    {
      break;
    }
  }

  int absolute_parameter_index = first_parameter_index + index;
  if(words.length() > absolute_parameter_index)
  {
    parameter = (ic_string *)words[absolute_parameter_index];
  }
  
  // Delete all ic_string objects except returning one.
  for(int i = 0; i < words.length(); ++i)
  {
    if(words[i] != (void *)parameter)
      delete (ic_string *)words[i];
  }

  return parameter;
}

/**
 * Extracts integer value from parameter.
 */
int rc_rasm::extract_int_parameter(ic_string *line, int index)
{
  ic_string *raw_parameter = extract_raw_parameter(line, index);
  int result = raw_parameter->to_i();
  delete raw_parameter;
  return result;
}

/**
 * Extracts double value from parameter.
 */
double rc_rasm::extract_float_parameter(ic_string *line, int index)
{
  ic_string *raw_parameter = extract_raw_parameter(line, index);
  double result = raw_parameter->to_f();
  delete raw_parameter;
  return result;
}

/**
 * Extracts string parameter from the line and parses all escape sequences
 * inside a parameter.
 */
ic_string *rc_rasm::extract_string_parameter(ic_string *line, int index)
{
  ic_string *raw_parameter = extract_raw_parameter(line, index);

  long length = raw_parameter->length();
  if(length < 2 || raw_parameter->char_at(0) != '"' ||
    raw_parameter->char_at(length - 1) != '"')
  {
    delete raw_parameter;
    ic_string *msg = ic_string::format(M_ERR_MALFORMED_STRING,
      raw_parameter->get());
    ERROR(msg, M_EMODE_COMPILE);
  }

  ic_string *string_parameter = raw_parameter->substr_get(1, length - 2);
  solve_escape_seq(string_parameter);

  delete raw_parameter;
  return string_parameter;
}

/**
 * Checks whether argument contains a int value or not.
 */
bool rc_rasm::is_parameter_int(ic_string *line, int index)
{
  ic_string *parameter = extract_raw_parameter(line, index);
  ic_regex int_regex = ic_regex("/^-?\\d+$/");
  ic_match *match = int_regex.match(parameter);
  delete parameter;
  if(match != NULL)
  {
    delete match;
    return true;
  }
  else
    return false;
}

/**
 * Checks whether argument contains a double value or not.
 */
bool rc_rasm::is_parameter_float(ic_string *line, int index)
{
  ic_string *parameter = extract_raw_parameter(line, index);
  ic_regex float_regex = ic_regex("/^-?\\d+\\.\\d+$/");
  ic_match *match = float_regex.match(parameter);
  delete parameter;
  if(match != NULL)
  {
    delete match;
    return true;
  }
  else
    return false;
}

/**
 * Checks whether argument contains a valid string constant or not.
 */
bool rc_rasm::is_parameter_string(ic_string *line, int index)
{
  ic_string *parameter = extract_raw_parameter(line, index);
  ic_regex int_regex = ic_regex("/\".*?(?<!\\\\)(?:\\\\\\\\)*\"/");
  ic_match *match = int_regex.match(parameter);
  delete parameter;
  if(match != NULL)
  {
    delete match;
    return true;
  }
  else
    return false;
}

/**
 * Processes all escape-sequences inside a string.
 * Parsed sequences are:
 * - "\n"         - new line
 * - "\r"         - carriage return
 * - "\t"         - tab symbol
 * - "\x[a-fA-F]" - symbol by its hex code
 */
void rc_rasm::solve_escape_seq(ic_string *string)
{
  long i = -1; // index of first unparsed slash symbol
  while((i = string->substr_first("\\", i+1)) != -1)
  {
    if(string->length() > i + 1)
    {
      char buff[2] = "";
      char escaped = '\0';
      switch(string->char_at(i + 1))
      {
      case 'n':
        escaped = '\n';
        break;
      case 't':
        escaped = '\t';
        break;
      case '\\':
        escaped = '\\';
        break;
      case 'x':
        if(string->length() >= i + 5)
        {
          ic_string *hex = new ic_string("0x");
          ic_string *buff = string->substr_get(i + 1, 2);
          hex->append(buff);
          delete buff;
          const char *tmp = hex->get();
          escaped = (char)strtol(tmp, NULL, 16);
          delete hex;
        }
        else
        {
          ic_string *msg = ic_string::format(M_ERR_BAD_ESCAPE_SEQ, string->get());
          ERROR(msg, M_EMODE_COMPILE);
        }
        break;
      default:
        {
          ic_string *msg = ic_string::format(M_ERR_BAD_ESCAPE_SEQ, string->get());
          ERROR(msg, M_EMODE_COMPILE);
        }
      }
      buff[0] = escaped;
      buff[1] = '\0';

      string->substr_set(i, 2, buff);
      i++;
    }
    else
    {
      ic_string *msg = ic_string::format(M_ERR_BAD_ESCAPE_SEQ, string->get());
      ERROR(msg, M_EMODE_COMPILE);
    }
  }
}

#endif // RC_RASM_H
