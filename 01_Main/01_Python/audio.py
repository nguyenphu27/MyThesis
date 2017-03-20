# pylint: disable=C0326, C1001
"""AUDIO
Author: Huy Vu
Date: Mar 20, 2017"""

from main_class import FLOW

HEALTHSYSTEM = FLOW()
print "PRINT FROM AUDIO MODULE"
# Check audio modules existence
if HEALTHSYSTEM.audio.exist_flag == 0:
    print "Missing Module Audio, E104"
    
else:


