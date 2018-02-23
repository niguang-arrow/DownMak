class SortedDict(dict):
    class Iterator(object):
        def __init__(self, sorted_dict):
            self._dict = sorted_dict
            self._keys = sorted(self._dict.keys())
            self._nr_items = len(self._keys)
            self._idx = 0

        def __iter__(self):
            return self

        def next(self):
            if self._idx >= self._nr_items:
                raise StopIteration

            key = self._keys[self._idx]
            value = self._dict[key]
            self._idx += 1
            return key, value

        __next__ = next

    def __iter__(self):
        return SortedDict.Iterator(self)

    iterkeys = __iter__

    def __str__(self):
        return str(sorted(self))


keyw = SortedDict()
keyw['exitAction'] = 'Ctrl+X'

if __name__ == '__main__':
    sdict = SortedDict()
    sdict['exitAction'] = 'Ctrl+X'
    sdict['deleteAction'] = 'Ctrl+D'
    sdict['aeleteAction'] = 'Ctrl+D'
    print sdict
    print keyw
