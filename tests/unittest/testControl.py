"""Unit tests for wx.Control.

Methods yet to test for wx.Control:
Command, Create, GetAlignment"""

import unittest
import wx

import wxtest
import testWindow

class ControlTest(testWindow.WindowTest):
    def setUp(self):
        self.app = wx.PySimpleApp()
        self.frame = wx.Frame(parent=None, id=wx.ID_ANY)
        self.testControl = wx.Control(parent=self.frame, id=wx.ID_ANY)
    
    def testAllControlsNeedParents(self):
        """__init__"""
        '''The assumption here is that all wx.Controls and subclasses need to have 
        parents.  Some platforms (GTK) do not enforce this at object creation
        time; this is to be considered an implementation detail.
        "wxWidgets does require that non top-level windows have a parent, 
        it's just not enforced in debug mode on wxGTK like the others do."
        For more information, see 
        http://lists.wxwidgets.org/cgi-bin/ezmlm-cgi?12:mss:3440:fjmhidphpdnbhoobomhi'''
        class_under_test = type(self.testControl)
        if wxtest.ASSERTIONS_ON:
            self.assertRaises(wx.PyAssertionError, class_under_test, None)
        else:
            class_under_test(None)

    def testGetName(self):
        """GetName"""
        name = 'Name of Control'
        class_under_test = type(self.testControl)
        ctrl = class_under_test(parent=self.frame, name=name)
        self.assertEquals(name, ctrl.GetName())
    
    def testLabelText(self):
        """SetLabel, GetLabelText"""
        # GetLabelText removes the mnemonics
        label = "Hello &World"
        labeltext = label.replace('&','')
        self.testControl.SetLabel(label)
        self.assertEquals(labeltext, self.testControl.GetLabelText())
    

if __name__ == '__main__':
    unittest.main()