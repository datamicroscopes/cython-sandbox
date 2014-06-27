from distributions.lp.models import bb as lp_bb
from distributions.io.schema_pb2 import BetaBernoulli as pb_bb

from microscopes.cxx._models cimport bb_factory

class py_model(object):
    def __init__(self, model_module, pb_type):
        self._model_module = model_module
        self._pb_type = pb_type

    def shared_dict_to_bytes(self, raw):
        s = self._model_module.Shared()
        s.load(raw)
        m = self._pb_type.Shared()
        s.dump_protobuf(m)
        return m.SerializeToString()

    def shared_bytes_to_dict(self, raw):
        m = self._pb_type.Shared()
        m.ParseFromString(raw)
        s = self._model_module.Shared()
        s.load_protobuf(m)
        return s.dump()

class bb_py_model(py_model):
    def __init__(self):
        super(bb_py_model, self).__init__(lp_bb, pb_bb)

bb = (bb_py_model(), bb_factory())
