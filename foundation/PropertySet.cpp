/*
	Scan Tailor - Interactive post-processing tool for scanned pages.
	Copyright (C) 2007-2009  Joseph Artsimovich <joseph_a@mail.ru>

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "PropertySet.h"
#include "PropertyFactory.h"
#include <QDomDocument>
#include <QDomElement>
#include <QString>
#include <boost/foreach.hpp>

PropertySet::PropertySet(QDomElement const& el, PropertyFactory const& factory)
{
	QString const property_str("property");
	QDomNode node(el.firstChild());

	for (; !node.isNull(); node = node.nextSibling()) {
		if (!node.isElement()) {
			continue;
		}
		if (node.nodeName() != property_str) {
			continue;
		}

		QDomElement prop_el(node.toElement());
		IntrusivePtr<Property> prop(factory.construct(prop_el));
		if (prop.get()) {
			m_props.push_back(prop);
		}
	}
}

QDomElement
PropertySet::toXml(QDomDocument& doc, QString const& name) const
{
	QString const property_str("property");
	QDomElement props_el(doc.createElement(name));

	BOOST_FOREACH(IntrusivePtr<Property> const& prop, m_props) {
		props_el.appendChild(prop->toXml(doc, property_str));
	}

	return props_el;
}

IntrusivePtr<PropertySet>
PropertySet::deepCopy() const
{
	IntrusivePtr<PropertySet> dst(new PropertySet);
	dst->m_props.reserve(m_props.size());

	BOOST_FOREACH(IntrusivePtr<Property> const& prop, m_props) {
		dst->m_props.push_back(prop->clone());
	}

	return dst;
}
