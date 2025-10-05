#include "texteditor.h"

#include <memory>
#include <stdexcept>

using namespace std;


class Node{
public:
	char data;
	shared_ptr<Node> next;


	Node(char c) : data(c), next(nullptr) {}
};


class TextEditorImpl : public TextEditor {
private:
	shared_ptr<Node> head;
	int cursorPos; // Cursor at 0 pos before first char and 1 after first one
	
public:
	TextEditorImpl() : head(nullptr), cursorPos(0) {}

	void insertChar(char c) override {
		if (c < 32 || c > 126) {
			throw invalid_argument("Invalid character");
		}

		auto newNode = make_shared<Node>(c);

		if (!head) {
			// First character
			head = newNode;

        } else if (cursorPos == 0) {
            // Insert at start
            newNode->next = head;
            head = newNode;
        
		} else {

			// Find the node before cursor pos
			auto current = head;
			for (int i = 0; i < cursorPos - 1 && current; i++) {
				current = current->next;

			}

			if (!current) {
				return; //// Beyond the end of text
			} else {
				// Insertion after curr node
				newNode->next = current->next;
				current->next = newNode;
			}
		}
		// Move after insertion
		cursorPos++;
	}


	void deleteChar() override {
		if (cursorPos == 0 || !head) {
			// Nothing to delete at start or empty text
			return;
		}

		if (cursorPos == 1) {
			// Delete first char
			head = head->next;

		} else {
			// Find node before the one to delete
			auto current = head;
			for (int i = 0; i < cursorPos - 2 && current; i++) {
				current = current->next;

			}

			current->next = current->next->next;
		}

		cursorPos--; // CUrsor moves back

	}

	void moveLeft() override {
		if (cursorPos > 0) {
			cursorPos--;
		}
	}


	void moveRight() override {
		// Count chars to know max cursor pos
		int length = 0;
		auto current = head;

		while (current) {
			length++;
			current = current->next;
		}

		if (cursorPos < length) {
		    cursorPos++;

		}
	}


	string getTextWithCursor() const override {
		string result;
		auto current = head;


		// Add chars before cursor
		for (int i = 0; i < cursorPos; i++) {
			if (current) {

				result += current->data;
				current = current->next;
			}
		}

		// Add cursor
		result += '|';


		// Add remaining chars after cursor
		while (current) {
			result += current->data;
			current = current->next;
		}

		return result;

	}
};

unique_ptr<TextEditor> TextEditor::create(){
	return make_unique<TextEditorImpl>();
}