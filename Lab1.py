# here in first list [x1,y1,x2,y2...], in secind [k,b]
import sys
sys.path.append('build/Release')
import CModule
import pytest 

# print((CModule.calc([0, 0, 0, 1, 1, 1, 1, 0], [1, 1])))

def test_calc_1 ():
    assert CModule.calc([0, 0, 0, 1, 1, 1, 1, 0], [1, -0.5]) == [1.0, 0.5, 1.0, 0.0, 0.5, -0.0]
    
def test_calc_2 ():
    assert CModule.calc([0, 0, 0, 1, 1, 1, 1, 0], [1, 0.5]) == [0.0, 0.0, 0.0, 0.5, 0.5, 1.0, 1.0, 1.0, 1.0, 0.0]
    
def test_calc_3 ():
    assert CModule.calc([0, 0, 0, 1, 1, 1, 1, 0], [1, 0]) == [0.0, 0.0, 1.0, 1.0, 1.0, 0.0]
    
def test_calc_4 ():
    assert CModule.calc([0, 0, 0, 1, 1, 1, 1, 0], [1, 1]) == [0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0]
    
def test_calc_5 ():
    assert CModule.calc([0, 0, 0, 1, 1, 1, 1, 0], [1, -1]) == [1.0, 0.0]   