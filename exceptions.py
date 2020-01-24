class Error(Exception):
    """base class"""
    pass

class NoFuncParamMatch(Error):
    """The given function does not match it's paramters"""
    pass
