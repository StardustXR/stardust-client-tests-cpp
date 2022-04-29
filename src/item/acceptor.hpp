#pragma once

#include "item.hpp"
#include <algorithm>
#include <stardustxr/fusion/types/fields/field.hpp>
#include <string_view>
#include <type_traits>

// template<class T, class B> struct Derived_from {
// 	static void constraints(T* p) { B* pb = p; }
// 	Derived_from() { void(*p)(T*) = constraints; }
// };

template<class T>
struct AcceptorItem {
	T *item;
	float distance;
	bool operator<(AcceptorItem<T> const& a) {
		return this->distance < a.distance;
	}
};

template<class T>
class ItemAcceptor {
public:
	explicit ItemAcceptor(StardustXRFusion::Field &field) {
		this->field = &field;
	}

	static void addItem(T *item) {
		items.push_back(AcceptorItem<T>{item, 0});
	}

	void update() {
		if(items.size() == 0)
			return;
		for (AcceptorItem<T> &acceptorItem : items) {
			AcceptorItem<T> *acceptorItemPtr = &acceptorItem;
			Item *item = acceptorItem.item;
			if(!item || item->getNodePath() == "/")
				continue;
			field->distance(item, Vec3::Zero, [acceptorItemPtr](float distance) {
				acceptorItemPtr->distance = distance;
			});
		}
		std::sort(items.begin(), items.end());
		for(AcceptorItem<T> &item : items) {
			Item *listItem = item.item;
			if(listItem && item.distance <= 0 && !listItem->isGrabbing() && listItem->grabbingChanged()) {
				onAcceptItem(static_cast<T *>(listItem));
				break;
			}
		}
	}

	void onAcceptItem(T *item) {
		Item *genericItem = item;
		genericItem->onAccepted(*field);
	}

protected:
	StardustXRFusion::Field *field;
	static std::vector<AcceptorItem<T>> items;
};

template <typename T> std::vector<AcceptorItem<T>> ItemAcceptor<T>::items(0);
