class Command:
    def __init__(self, function_name, *args):
        self.function_name = function_name
        self.params = args

    def execute(self):
        self.function_name(*self.params)

    @property
    def function_name(self):
        return self.__function_name

    @function_name.setter
    def function_name(self, function_name):
        self.__function_name = function_name

    @property
    def params(self):
        return self.__params

    @params.setter
    def params(self, params):
        self.__params = params

    def __str__(self):
        return "Function: " + str(self.function_name) + " with parameters: " + str(self.params)

class Operation:
    def __init__(self, undo_action: Command, redo_action: Command):
        self.undo_action = undo_action
        self.redo_action = redo_action

    def undo(self):
        self.undo_action.execute()

    def redo(self):
        self.redo_action.execute()

    def __str__(self):
        return "Operation: " + str(self.undo_action) + " & " + str(self.__redo_action)

    @property
    def undo_action(self):
        return self.__undo_action

    @undo_action.setter
    def undo_action(self, undo_action):
        self.__undo_action = undo_action

    @property
    def redo_action(self):
        return self.__redo_action

    @redo_action.setter
    def redo_action(self, redo_action):
        self.__redo_action = redo_action

class CascadedOperation:
    def __init__(self, operation_list: list[Operation]):
        self.operation_list = operation_list

    def undo(self):
        for op in self.operation_list:
            op.undo()

    def redo(self):
        for op in reversed(self.operation_list):
            op.redo()

    def __str__(self):
        return "Cascaded operation: " + str(self.operation_list)

    @property
    def operation_list(self):
        return self.__operation_list

    @operation_list.setter
    def operation_list(self, operation_list):
        self.__operation_list = operation_list

class UndoRedoException(Exception):
    pass

class UndoRedoService:
    def __init__(self):
        self.undo_stack = []
        self.redo_stack = []
        self.is_undoredo_op = False

    def record_for_undo(self, operation):
        if self.is_undoredo_op:
            return
        self.undo_stack.append(operation)

    def undo(self):
        if len(self.undo_stack) == 0:
            raise UndoRedoException("No more undos!")
        self.is_undoredo_op = True
        operation = self.undo_stack.pop()
        self.redo_stack.append(operation)
        operation.undo()
        self.is_undoredo_op = False

    def redo(self):
        if len(self.redo_stack) == 0:
            raise UndoRedoException("No more redos!")
        self.is_undoredo_op = True
        operation = self.redo_stack.pop()
        self.undo_stack.append(operation)
        operation.redo()
        self.is_undoredo_op = False

    @property
    def undo_stack(self):
        return self.__undo_stack

    @undo_stack.setter
    def undo_stack(self, undo_stack):
        self.__undo_stack = undo_stack

    @property
    def redo_stack(self):
        return self.__redo_stack

    @redo_stack.setter
    def redo_stack(self, redo_stack):
        self.__redo_stack = redo_stack
