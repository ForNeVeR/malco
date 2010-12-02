# -*- coding: utf-8 -*-

import re
from pygments.lexer import RegexLexer, bygroups, include
from pygments.token import *

class MalcoLexer(RegexLexer):
    name = 'Malco programming language lexer'
    aliases = ['malco']
    filenames = ['*.mlc']
    mimetypes = ['text/malco']
    
    flags = re.DOTALL
    tokens = {
        'whitespaceandcomments': [
            # Special for wiki bad code highlighting
            (r'//\s*BAD\s*CODE\s*\n', Generic.Error),
            
            # Comments
            (r'//.*?\n', Comment.Single),
            (r'/\*.*?\*/', Comment.Multiline),
            
            # Whitespace
            (r'\s', Text),
        ],
        
        'slashstartsregex': [
            # If we're in this state, any slash may start regex.
            include('whitespaceandcomments'),
            
            # RegExes
            (r'/.*?(?<!\\)(\\\\)*/([ismx]+\b)?', String.Regex),
            
            # Condition to leave state:
            (r'(?=.)', Generic.Error, '#pop'),
        ],
        
        'root': [
            include('whitespaceandcomments'),

            # Keywords
            (r'(if|elseif|else|for|while|switch|case|break|try|catch|throw|in)\b', Keyword),
            (r'(class|extend|func|op)\b', Keyword.Declaration),
            (r'(public|private|internal|static|stub|final)\b', Keyword),
            (r'(include|require)\b', Keyword.Namespace),
            (r'once\b', Keyword),
            (r'(new|return)\b', Keyword),
            
            # Literals
            (r'(undef|false|true)\b', Keyword.Constant), # mcUndef, mcBool
            (r'-?[0-9]+\b', Number.Integer), # mcInteger
            (r'-?0x[0-9a-fA-F]+\b', Number.Hex), # mcInteger
            (r'-?0b[01]+\b', Number), # mcInteger
            (r'-?[0-9]+\.[0-9]+(e(\+|-)[0-9]+)?\b', Number.Float), # mcFloat
            
            # Strings
            (r'".*?(?<!\\)(\\\\)*"', String), # mcString
            (r"'.*?(?<!\\)(\\\\)*'", String), # mcString
            
            # Variables
            (r'\$@?\w*', Name.Variable),
            
            # Operators
            # 3-char operators
            (r'(===|!==|<=>)', Operator, 'slashstartsregex'), # The longest comparison operators
            # 2-char operators
            (r'(\+|-|\*|/|%|\^|[!&\|~])=', Operator, 'slashstartsregex'), # Various assignments
            (r'\.\.', Operator, 'slashstartsregex'), # mcInterval double-dot operator
            (r'(\+\+|--)', Operator), # Increment / decrement
            (r'(&&|\|\|)', Operator, 'slashstartsregex'), # Logical and / or
            (r'(==|!=|>=|<=)', Operator, 'slashstartsregex'), # Long comparison operators
            (r'<<', Operator, 'slashstartsregex'), # String concatenation operator
            (r'::', Operator), # Access operator
            (r'\?:', Operator, 'slashstartsregex'), # Value finder
            # Single-char operators
            (r'(\+|-|\*|/|%|\^)', Operator, 'slashstartsregex'), # Simple arithmetic operators
            (r'[!&\|]', Operator, 'slashstartsregex'), # Binary not, and (also link creation), or
            (r'xor', Operator.Word, 'slashstartsregex'),
            (r'[<>]', Operator, 'slashstartsregex'), # Simple comparison operators
            (r'~', Operator, 'slashstartsregex'), # Relativity operator
            (r'\.', Operator), # Member access operator
            (r'[\?:]', Operator, 'slashstartsregex'), # Ternary operator parts
            (r'=', Operator, 'slashstartsregex'), # Simple assignment
           
            # Braces and symbols
            (r'[\({\[,;]', Punctuation, 'slashstartsregex'),
            (r'[\)}\]@]', Punctuation),
            
            # Just names
            (r'(\w+!)(\s*)(\()', bygroups(Name, Text, Punctuation)),
            (r'\w+', Name),
        ],
    }

# Test usage:
# > python malco.py > malco.html
# After that open malco.html in browser.
if __name__ == '__main__':
    from pygments import highlight
    from pygments.formatters import HtmlFormatter

    code = r'''
//                  BAD CODE
/*
 * Tests for various strings
 */
$a = "just string";
$b = "multi-line
      string";
$c = "some tricky\n
      escapes\t
      \"
      'this string is just a part of another string'\\";
// Time for some regexes!
$re  = /zomg/i;       // zomg
$re2 = /zomg\/zomg/i; // wtf
$re3 = /a
        b
        c/;           // da coment
$re4 = //i;           // TODO: Fix this (it must be a regex)
// And some small arithmetics
$a = 3 + 2 - 1;                         // 4
$b = 3 * 2 / 4;                         // 1.5
$c = 5 % 2;                             // remainder: 1
$d = 2 ^ 3;                             // power: 8
'''

    print '<style>' + HtmlFormatter().get_style_defs('.highlight') + '</style>'
    print highlight(code, MalcoLexer(), HtmlFormatter())