# -*- coding: utf-8 -*-
'''
Malco and Radix assembler lexers for pygments.
'''
from setuptools import setup

__author__ = 'ForNeVeR'

setup(
    name='malco_lexers',
    version='0.2',
    description=__doc__,
    author=__author__,
    packages=['malco_lexers'],
    entry_points='''
    [pygments.lexers]
    RasmLexer = malco_lexers.rasm:RasmLexer
    MalcoLexer = malco_lexers.malco:MalcoLexer
    '''
)