# -*- coding: koi8-r -*-

import unittest
from test.support import TESTFN, unlink, unload
import importlib
import os
import sys
import subprocess

class SourceEncodingTest(unittest.TestCase):

    def test_pep263(self):
        self.assertEqual(
            "鹕韵