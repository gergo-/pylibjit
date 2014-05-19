import unittest
import math
import jit

class TestTypes(unittest.TestCase):
    def test_static_types(self):
        self.assertEqual(jit.Type.void.type, jit.TYPES.VOID) 
        self.assertEqual(jit.Type.sbyte.type, jit.TYPES.SBYTE)
        self.assertEqual(jit.Type.ubyte.type, jit.TYPES.UBYTE)
        self.assertEqual(jit.Type.short.type, jit.TYPES.SHORT)
        self.assertEqual(jit.Type.ushort.type, jit.TYPES.USHORT)
        self.assertEqual(jit.Type.int.type, jit.TYPES.INT)
        self.assertEqual(jit.Type.uint.type, jit.TYPES.UINT)
        self.assertEqual(jit.Type.nint.type, jit.TYPES.NINT)
        self.assertEqual(jit.Type.nuint.type, jit.TYPES.NUINT)
        self.assertEqual(jit.Type.long.type, jit.TYPES.LONG)
        self.assertEqual(jit.Type.ulong.type, jit.TYPES.ULONG)
        self.assertEqual(jit.Type.float32.type, jit.TYPES.FLOAT32)
        self.assertEqual(jit.Type.float64.type, jit.TYPES.FLOAT64)
        self.assertEqual(jit.Type.nfloat.type, jit.TYPES.NFLOAT)

    def test_binary_comparison_operators(self):
        self.assertTrue(jit.Type.int == jit.Type.int)
        self.assertFalse(jit.Type.int != jit.Type.int)
        self.assertTrue(jit.Type.int != jit.Type.uint)
        self.assertFalse(jit.Type.int == jit.Type.uint)

    def test_union(self):
        union = jit.Type.create_union([jit.Type.int, jit.Type.long, jit.Type.float32])
        self.assertEqual(union.type, jit.TYPES.UNION)
        self.assertTrue(union.is_union) 
        self.assertEqual(union.num_fields, 3)

    def test_struct(self):
        struct = jit.Type.create_struct([jit.Type.int, jit.Type.long, jit.Type.float32])
        self.assertEqual(struct.type, jit.TYPES.STRUCT)
        self.assertTrue(struct.is_struct) 
        self.assertEqual(struct.num_fields, 3)

    def test_tagged(self):
        tagged = jit.Type.create_tagged(jit.Type.int, 1, 'tag')
        self.assertGreater(tagged.type, jit.TYPES.FIRST_TAGGED)
        self.assertTrue(tagged.has_tag, 0)
        self.assertEqual(tagged.get_tagged_type(), jit.Type.int)
        self.assertEqual(tagged.tagged_data, 'tag')

        tagged.set_tagged_type(jit.Type.float32)
        self.assertEqual(tagged.get_tagged_type(), jit.Type.float32)

        self.assertEqual(tagged.remove_tags(), jit.Type.float32)

    def test_normalize(self):
        self.assertEqual(jit.Type.nint.normalize(), jit.Type.long)
        self.assertEqual(jit.Type.nuint.normalize(), jit.Type.ulong)
        self.assertEqual(jit.Type.nfloat.normalize(), jit.Type.nfloat)


class TestFunctions(unittest.TestCase):
    def setUp(self):
        self.context = jit.Context()

    # signature_helper
    def test_signature_helper(self):
        sig = jit.Function.signature_helper(jit.Type.int, jit.Type.int, jit.Type.int, jit.Type.int)
        self.assertTrue(sig.is_signature)
        self.assertEqual(sig.num_params, 3)

    # constructors
    def test_constructors(self):
        sig = jit.Function.signature_helper(jit.Type.void)

        self.assertRaises(TypeError, jit.Function)
        self.assertTrue(jit.Function(self.context))
        self.assertTrue(jit.Function(self.context, sig))

    def test_copy_constructor(self):
        func = jit.Function(self.context)
        self.assertTrue(jit.Function(func))

    # protected functions
    def test_protected_functions(self):
        func = jit.Function(self.context)
        self.assertRaises(TypeError, func.build)
        self.assertRaises(TypeError, func.create_signature)
        self.assertRaises(TypeError, func.fail)
        self.assertRaises(TypeError, func.out_of_memory)

class TestFunctionsMethods(unittest.TestCase):
    def setUp(self):
        self.context = jit.Context()

    def test_int_constant(self):
        class function(jit.Function):
            def __init__(self, context):
                super().__init__(context)
                self.create()
            def create_signature(self):
                return self.signature_helper(jit.Type.int)
            def build(self):
                self.insn_return(self.new_constant(2000, jit.Type.int))

        func = function(self.context)
        self.assertEqual(func(), 2000)

    def test_uint_constant(self):
        class function(jit.Function):
            def __init__(self, context):
                super().__init__(context)
                self.create()
            def create_signature(self):
                return self.signature_helper(jit.Type.uint)
            def build(self):
                self.insn_return(self.new_constant(2, jit.Type.uint))

        func = function(self.context)
        self.assertEqual(func(), 2)

    def test_sbyte_constant(self):
        class function(jit.Function):
            def __init__(self, context):
                super().__init__(context)
                self.create()
            def create_signature(self):
                return self.signature_helper(jit.Type.sbyte)
            def build(self):
                self.insn_return(self.new_constant(127, jit.Type.sbyte))

        func = function(self.context)
        self.assertEqual(func(), 127)

    def test_sbyte_limits(self):
        class function(jit.Function):
            def __init__(self, context):
                super().__init__(context)
                self.create()
            def create_signature(self):
                return self.signature_helper(jit.Type.sbyte, jit.Type.sbyte)
            def build(self):
                self.insn_return(self.new_constant(0, jit.Type.sbyte))

        self.assertRaises(TypeError, function(self.context), 128)
        self.assertRaises(TypeError, function(self.context), -128)

    def test_short_constant(self):
        class function(jit.Function):
            def __init__(self, context):
                super().__init__(context)
                self.create()
            def create_signature(self):
                return self.signature_helper(jit.Type.uint)
            def build(self):
                self.insn_return(self.new_constant(2000, jit.Type.short))

        func = function(self.context)
        self.assertEqual(func(), 2000)

    def test_long_constant(self):
        class function(jit.Function):
            def __init__(self, context):
                super().__init__(context)
                self.create()
            def create_signature(self):
                return self.signature_helper(jit.Type.int)
            def build(self):
                self.insn_return(self.new_constant(2000, jit.Type.long))

        func = function(self.context)
        self.assertEqual(func(), 2000)

    def test_float32_constant(self):
        class function(jit.Function):
            def __init__(self, context):
                super().__init__(context)
                self.create()
            def create_signature(self):
                return self.signature_helper(jit.Type.float32)
            def build(self):
                self.insn_return(self.new_constant(math.pi))

        func = function(self.context)
        self.assertAlmostEqual(func(), math.pi, places=6)

    def test_float64_constant(self):
        class function(jit.Function):
            def __init__(self, context):
                super().__init__(context)
                self.create()
            def create_signature(self):
                return self.signature_helper(jit.Type.float64)
            def build(self):
                self.insn_return(self.new_constant(math.pi, jit.Type.float64))

        func = function(self.context)
        self.assertAlmostEqual(func(), math.pi)

    def test_mul_add_int_constants(self):
        class function(jit.Function):
            def __init__(self, context):
                super().__init__(context)
                self.create()
            def create_signature(self):
                return self.signature_helper(jit.Type.int)
            def build(self):
                x = self.new_constant(2)
                y = self.new_constant(3)
                z = self.new_constant(4)
                self.insn_return(x * y + z)

        func = function(self.context)
        self.assertEqual(func(), (2*3+4))

    def test_mul_add_float32_constants(self):
        class function(jit.Function):
            def __init__(self, context):
                super().__init__(context)
                self.create()
            def create_signature(self):
                return self.signature_helper(jit.Type.float32)
            def build(self):
                x = self.new_constant(2.0)
                y = self.new_constant(3.0)
                z = self.new_constant(4.0)
                self.insn_return(x * y + z)

        func = function(self.context)
        self.assertEqual(func(), (2.0 * 3.0 + 4.0))

    def test_mul_add_float64_constants(self):
        class function(jit.Function):
            def __init__(self, context):
                super().__init__(context)
                self.create()
            def create_signature(self):
                return self.signature_helper(jit.Type.float64)
            def build(self):
                x = self.new_constant(2.0)
                y = self.new_constant(3.0)
                z = self.new_constant(4.0)
                self.insn_return(x * y + z)

        func = function(self.context)
        self.assertEqual(func(), (2.0 * 3.0 + 4.0))

    def test_mul_add_nfloat_constants(self):
        class function(jit.Function):
            def __init__(self, context):
                super().__init__(context)
                self.create()
            def create_signature(self):
                return self.signature_helper(jit.Type.nfloat)
            def build(self):
                x = self.new_constant(2.0)
                y = self.new_constant(3.0)
                z = self.new_constant(4.0)
                self.insn_return(x * y + z)

        func = function(self.context)
        self.assertEqual(func(), (2.0 * 3.0 + 4.0))

    def test_mul_add_int(self):
        class function(jit.Function):
            def __init__(self, context):
                super().__init__(context)
                self.create()
            def create_signature(self):
                return self.signature_helper(jit.Type.int, jit.Type.int, jit.Type.int, jit.Type.int)
            def build(self):
                x, y, z = [self.get_param(i) for i in range(3)]
                self.insn_return(x * y + z)

        func = function(self.context)
        self.assertEqual(func(2,3,4), (2*3+4))

    def test_mul_add_nint(self):
        class function(jit.Function):
            def __init__(self, context):
                super().__init__(context)
                self.create()
            def create_signature(self):
                return self.signature_helper(jit.Type.nint,
                                             jit.Type.nint, jit.Type.nint, jit.Type.nint)
            def build(self):
                x, y, z = [self.get_param(i) for i in range(3)]
                self.insn_return(x * y + z)

        func = function(self.context)
        self.assertEqual(func(2,3,4), (2*3+4))

    def test_mul_add_uint(self):
        class function(jit.Function):
            def __init__(self, context):
                super().__init__(context)
                self.create()
            def create_signature(self):
                return self.signature_helper(jit.Type.uint,
                                             jit.Type.uint, jit.Type.uint, jit.Type.uint)
            def build(self):
                x, y, z = [self.get_param(i) for i in range(3)]
                self.insn_return(x * y + z)

        func = function(self.context)
        self.assertEqual(func(2,3,4), (2*3+4))

    def test_mul_add_long(self):
        class function(jit.Function):
            def __init__(self, context):
                super().__init__(context)
                self.create()
            def create_signature(self):
                return self.signature_helper(jit.Type.long,
                                             jit.Type.long, jit.Type.long, jit.Type.long)
            def build(self):
                x, y, z = [self.get_param(i) for i in range(3)]
                self.insn_return(x * y + z)

        func = function(self.context)
        self.assertEqual(func(2,3,4), (2*3+4))

    def test_mul_add_ulong(self):
        class function(jit.Function):
            def __init__(self, context):
                super().__init__(context)
                self.create()
            def create_signature(self):
                return self.signature_helper(jit.Type.ulong,
                                             jit.Type.ulong, jit.Type.ulong, jit.Type.ulong)
            def build(self):
                x, y, z = [self.get_param(i) for i in range(3)]
                self.insn_return(x * y + z)

        func = function(self.context)
        self.assertEqual(func(2,3,4), (2*3+4))

    def test_mul_add_nfloat(self):
        class function(jit.Function):
            def __init__(self, context):
                super().__init__(context)
                self.create()
            def create_signature(self):
                return self.signature_helper(jit.Type.nfloat,
                                             jit.Type.nfloat, jit.Type.nfloat, jit.Type.nfloat)
            def build(self):
                x, y, z = [self.get_param(i) for i in range(3)]
                self.insn_return(x * y + z)

        func = function(self.context)
        self.assertEqual(func(2.0, 3.0, 4.0), (2.0 * 3.0 + 4.0))

    def test_mul_add_float32(self):
        class function(jit.Function):
            def __init__(self, context):
                super().__init__(context)
                self.create()
            def create_signature(self):
                return self.signature_helper(jit.Type.float32,
                                             jit.Type.float32, jit.Type.float32, jit.Type.float32)
            def build(self):
                x, y, z = [self.get_param(i) for i in range(3)]
                self.insn_return(x * y + z)

        func = function(self.context)
        self.assertEqual(func(2.0, 3.0, 4.0), (2.0 * 3.0 + 4.0))

    def test_mul_add_float64(self):
        class function(jit.Function):
            def __init__(self, context):
                super().__init__(context)
                self.create()
            def create_signature(self):
                return self.signature_helper(jit.Type.float64,
                                             jit.Type.float64, jit.Type.float64, jit.Type.float64)
            def build(self):
                x, y, z = [self.get_param(i) for i in range(3)]
                self.insn_return(x * y + z)

        func = function(self.context)
        self.assertEqual(func(2.0, 3.0, 4.0), (2.0 * 3.0 + 4.0))

    def test_raw_mul_add_int(self):
        sig = jit.Function.signature_helper(jit.Type.int, jit.Type.int, jit.Type.int, jit.Type.int)
        func = jit.Function(self.context, sig)

        func.build_start()

        x, y, z = [func.get_param(i) for i in range(3)]
        func.insn_return(x * y + z)

        func.compile()
        func.build_end()

        self.assertEqual(func(2, 3, 4), (2 * 3 + 4))

    def test_context_manager(self):
        sig = jit.Function.signature_helper(jit.Type.int, jit.Type.int, jit.Type.int, jit.Type.int)
        func = jit.Function(self.context, sig)

        with func as f:
            x, y, z = [f.get_param(i) for i in range(3)]
            f.insn_return(x * y + z)

            f.compile()

        self.assertEqual(func(2, 3, 4), (2 * 3 + 4))


class TestFunctionsOps(unittest.TestCase):
    def setUp(self):
        self.context = jit.Context()

    def test_insn_abs(self):
        sig = jit.Function.signature_helper(jit.Type.int, jit.Type.int)
        func = jit.Function(self.context, sig)
        with func as f:
            x = f.get_param(0)
            f.insn_return(f.insn_abs(x))
            f.compile()

        self.assertEqual(func(-2), 2)

    def test_insn_acos(self):
        sig = jit.Function.signature_helper(jit.Type.float64, jit.Type.float64)
        func = jit.Function(self.context, sig)
        with func as f:
            x = f.get_param(0)
            f.insn_return(f.insn_acos(x))
            f.compile()

        self.assertEqual(func(0.12), math.acos(0.12))

class TestContext(unittest.TestCase):
    def setUp(self):
        self.context = jit.Context()

    def tearDown(self):
        del self.context

    def test_context_functions0(self):
        class function(jit.Function):
            def __init__(self, context):
                super().__init__(context)
                self.create()
            def create_signature(self):
                return self.signature_helper(jit.Type.uint)
            def build(self):
                self.insn_return(self.new_constant(2, jit.Type.uint))

        func = function(self.context)
        self.assertEqual(self.context.functions[-1](), 2)

    def test_context_functions1(self):
        sig = jit.Function.signature_helper(jit.Type.float64, jit.Type.float64)
        for i in range(10):
            func = jit.Function(self.context, sig)
            func.build_start()
            x = func.get_param(0)
            func.insn_return(func.insn_acos(x))
            func.compile()
            func.build_end()

        self.assertEqual(len(self.context.functions), 10)

        for i in range(10):
            self.assertEqual(self.context.functions[i](0.12), math.acos(0.12))

if __name__ == '__main__':
    unittest.main()
