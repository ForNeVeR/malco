# -*- coding: utf-8 -*-

from pygments.lexer import RegexLexer, bygroups
from pygments.token import *

class RasmLexer(RegexLexer):
    name = 'Radix assembler language lexer'
    aliases = ['rasm']
    filenames = ['*.rasm']
    mimetypes = ['text/rasm']

    tokens = {
        'root': [
            # Whitespace and comments
            (r'\s+', Text),
            (r';.*?$', Comment.Single),
            
            # Commands, tables, modifiers
            (r'(LOADAX|LOADBX|SAVEAX|SAVEBX|XCHG|ASSIGN|UNSPLASSIGN|ADD|SUB|MUL|DIV|MOD|POW|SHL|SHR|BAND|BOR|BXOR|AND|OR|XOR|INC|DEC|NEG|EQ|EQ_STRICT|REL|LESS|LESS_EQ|GREATER|GREATER_EQ|CMP|JTRUE|JFALSE|JMP|PUSHUS|POPUS|PUSHSRC|POPSRC|PUSHDST |POPDST|SPLAT|UNSPLAT|CLRSRC|CLRDST|NEW|CALL|RETURN|NSP|BINDLAMBDA|INDEX|INCLUDE|REQUIRE|GC|SETPTY|SETFILE|SETLINE |THROW|TRY|TRIED|EXIT|REGCLASS|REGPROPERTY|REGMETHOD|INSPECT)\b', bygroups(Name.Builtin)),
            (r'(INT|FLOAT|STR|VAL)', Keyword.Type),
            
            # Pseudo commands (more complicated than others)
            (r'(LABEL|FUNC|CLASS|VAR|END)(\s+)(final|stub|static|private|public)?', bygroups(Name.Builtin.Pseudo, Text, Keyword.Reserved)),
            
            # Data
            (r'\d+(\.\d*)?', Number),
            (r'\@?\$\S*', Name.Variable),
            (r'".*?(?<!\\)(\\\\)*"', String),
        ]
    }

# Test usage:
# > python rasm.py > rasm.html
# After that open rasm.html in browser.
if __name__ == '__main__':
    from pygments import highlight
    from pygments.formatters import HtmlFormatter

    code = r'''
LOADAX    0
PUSHIN           ; generate $a
LOADAX    1
PUSHIN
UNSPLATIN
SAVEAX    $a

LOADAX "string\n\""
LOADBX "string\\\\"
'''

    print '<style>' + HtmlFormatter().get_style_defs('.highlight') + '</style>'
    print highlight(code, RasmLexer(), HtmlFormatter())