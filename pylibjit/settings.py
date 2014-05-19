# Inline reference counting operations? Set to True for performance, False
# for more readable code (for debugging).
inline_rc = True
# Generate placeholder nop calls for elided reference count operations? Set
# to False for performance, True for code that is better diffable (for
# debugging).
# rc_placeholders = True
rc_placeholders = not inline_rc

# reference_counting = True
reference_counting = False
allocate_frames = False
# allocate_frames = True
fully_dynamic_typing = False
# fully_dynamic_typing = True
boxed_numbers = False
# boxed_numbers = True
# static_number_typing = False
static_number_typing = True
bounds_checking = False
# bounds_checking = True

# Insert spurious load operations for function calls the compiler can
# resolve statically? Set to True to simulate Python's behavior, False for
# performance.
late_binding = False
