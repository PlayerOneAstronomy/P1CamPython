# -*- coding: utf-8 -*-

import player_one_camera as P1Cam


def test_main():
    assert P1Cam.__version__ == "0.0.1"
    assert P1Cam.GetErrorString(P1Cam.Errors.OK) == "operation successfully"
    assert P1Cam.GetAPIVersion() > 20230101
