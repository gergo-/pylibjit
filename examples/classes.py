# pylibjit can be used to statically resolve method calls in object-oriented
# code. A call from a compiled method to a compiled method (in the same or
# in a different class) bypasses all dynamic lookups if the dynamic type of
# the receiver is declared. Note that all instances at runtime must be of
# exactly the declared type, *not* a subclass.

import jit, pylibjit

class Foo:
    @pylibjit.compile(return_type=object,
                      argument_types=[object],
                      dump_code=False)
    def hello(self):
        print('hi there from Foo')

    # The call self.hello() will be statically bound to Foo.hello because
    # - say_hello is compiled;
    # - the receiver (self) is declared to be an instance of exactly this
    #   class (Foo); and
    # - Foo.hello is compiled.
    # The use of pylibjit.this_class() is a hack that is necessary because the
    # name Foo is not visible in the global namespace at this point, only
    # after the class definition is finished.
    @pylibjit.compile(return_type=object,
                      argument_types=[pylibjit.this_class()],
                      dump_code=False)
    def say_hello(self):
        print('will call Foo.hello')
        self.hello()

class Bar:
    # This call to Foo.hello will be resolved statically, as above.
    @pylibjit.compile(return_type=object,
                      argument_types=[object, Foo],
                      dump_code=False)
    def say_hello_foo(self, some_foo_object):
        print('will call Foo.hello from Bar')
        some_foo_object.hello()

    def hello(self):
        print('hello from Bar')

    # This looks like Foo.say_hello, but the call to Bar.hello will *not* be
    # resolved statically because Bar.hello is not a compiled function.
    @pylibjit.compile(return_type=object,
                      argument_types=[pylibjit.this_class()],
                      dump_code=False)
    def say_hello(self):
        print('will call Bar.hello')
        self.hello()

foo = Foo()
bar = Bar()

foo.say_hello()
bar.say_hello_foo(foo)
bar.say_hello()
