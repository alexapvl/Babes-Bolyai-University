#include "../include/ui/ui.h"
#include "../include/tests/tests.h"

int main(){
    TestAll();
    Repository* repository = createRepository(10);
    UndoRedoRepository* undoRedoRepository = createUndoRedoRepository();
    Service* service = createService(repository, undoRedoRepository);
    UI* ui = createUI(service);
    start(ui);
    destroyUI(ui);
    return 0;
}
