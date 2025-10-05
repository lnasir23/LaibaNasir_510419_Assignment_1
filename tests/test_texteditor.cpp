#include "texteditor.h"

#include <iostream>
#include <memory>

using namespace std;

void testTextEditor() {
    
    auto ed = TextEditor::create();
    
    cout << "Start: " << ed->getTextWithCursor() << endl;
    
    ed->insertChar('a');
    cout << "After insert 'a': " << ed->getTextWithCursor() << endl;
    
    ed->insertChar('b');
    cout << "After insert 'b': " << ed->getTextWithCursor() << endl;
    
    ed->moveLeft();
    cout << "After move left: " << ed->getTextWithCursor() << endl;
    
    ed->insertChar('c');
    cout << "After insert 'c': " << ed->getTextWithCursor() << endl;
    
    ed->deleteChar();
    cout << "After delete: " << ed->getTextWithCursor() << endl;
    
    ed->moveLeft();
    ed->moveLeft();
    cout << "After move left twice: " << ed->getTextWithCursor() << endl;
    
    ed->moveRight();
    ed->moveRight();
    cout << "After move right twice: " << ed->getTextWithCursor() << endl;
}

int main() {
    testTextEditor();
    return 0;
}