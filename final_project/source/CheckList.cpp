#include "stdafx.h"
#include "CheckList.h"
#include "Focuslist.h"
#include <string.h>

//convert string to wstring
wstring CheckList::s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	wstring r(buf);
	delete[] buf;
	return r;
}

CheckList::CheckList(int width, int height, vector<string> entries) :Panel((height*entries.size()), width), _entries(entries){
	wstring stemp;
	for (int i = 0; i < entries.size(); i++) {
	Focuslist* list = new Focuslist(width);
	list->setHeight(height);
	list->setBackground(Color::White);
	list->setForeground(Color::Black);
		stemp = s2ws(entries[i]);
		list->setText(stemp);
		list->addListener(*new Choice_button);
		this->_controls.push_back(list);
	}
	_cursorPositiony = _controls[0]->getTop() + 1;


}

void CheckList::SelectedIndex(size_t index) {

	_index.push_back(index);
	//turn on background on selected
	_controls[index]->setBackground(Color::Green);

}

void CheckList::DeSelectedIndex(size_t index) {
	
	for (int i = 0; i < _index.size(); i++) {
		if (_index[i] == index) {
			_index.erase(_index.begin() + i);
		//turn off backgound
			_controls[index]->setBackground(Color::White);
		}

	}

}
//return background to original after scroling down throuth the list
void CheckList::restCursor() {
	for (int i = 0; i < _controls.size(); i++) {

		_controls[i]->setBackground(Color::White);
		_controls[i]->setForeground(Color::Black);

		for (int j = 0; j < _index.size(); j++) {
			if (_index[j] == i) {
				_controls[i]->setBackground(Color::Green);
				_controls[i]->setForeground(Color::Black);
				break;
			}
		}
	}
}

//to mark to orang while scorling through the list
void CheckList:: keyDown(int keyCode, char charecter) {
	if (this->isOpened()) {
		auto f = Control::getFocus();
		auto it = find(_controls.begin(), _controls.end(), f);
		if (keyCode == VK_DOWN) {
			if (it != _controls.end() && ++it!=_controls.end()) {
				Control::setFocus(**it);
				restCursor();
				(**it).setBackground(Color::Orange);
				(**it).setForeground(Color::White);
			}
		}

		else if (keyCode == VK_UP) {
			if (it != _controls.begin()) {
				it--;
				Control::setFocus(**it);
				restCursor();
				(**it).setBackground(Color::Orange);
				(**it).setForeground(Color::White);
			}

		}
		else if (keyCode == VK_RETURN || keyCode == VK_SPACE) {
			int lock = 0;
			int pos =it - _controls.begin();
			Control::getFocus()->setForeground(Color::Black);
				for (int j = 0; j < _index.size(); j++) {
					if (_index[j] == pos) {
						f->setBackground(Color::White);
						_index.erase(_index.begin() + j);
						lock = 1;
						break;
					}
				}
				if (lock == 0) {
					f->setBackground(Color::Green);
					_index.push_back(pos);
				}
		}


	}
}

//in case we pressed the mouse
void CheckList::mousePressed(int x, int y, bool isLeft) {
	opened = true;
	x -= _left;
	y -= _top;
	for (int i = 0; i < _controls.size(); i++) {
		int myy = _controls[i]->getTop() + _controls[i]->getHeight();
		int y_l = _controls[i]->getTop();
		if (y >= y_l && y <= myy) {
			_cursorPositiony = _top + _controls[i]->getTop()+1;
			Focuslist* tmp = static_cast<Focuslist*>(_controls[i]);
			Control::setFocus(*_controls[i]);
			tmp->getListener().MousePressed(*this, i, y, isLeft);
			break;
		}
	}
}

//what to do in click case
void CheckList::click(size_t index) {
	int lock = 0;

	for (int i = 0; i < _index.size(); i++) {
		if (_index[i] == index) {
			DeSelectedIndex(index);
			lock = 1;
		}
	}
	if(lock==0)
		SelectedIndex(index);
}

//left
void CheckList::setLeft(int left) {
	_left = left;
	for (int i = 0; i < _controls.size(); i++) {
		_controls[i]->setLeft(0);
	}

}

//top
void CheckList::setTop(int top) {
	_top = top;
	for (int i = 0; i < _controls.size(); i++) {
		if (i == 0) {
			_controls[i]->setTop(0);
		}
		else {
			_controls[i]->setTop(_controls[i - 1]->getHeight() + _controls[i - 1]->getTop());
		}
	}

}