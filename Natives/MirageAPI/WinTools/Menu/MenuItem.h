#pragma once

namespace MirageAPI
{
    public ref class MenuItem
    {
        MenuItemType m_type;

    public:
        int Id;
        String^ Text;

        MenuItem(
            MenuItemType type,
            int id,
            String^ text
        ) : m_type(type),
            Id(id),
            Text(text)
        {
        }

        property MenuItemType Type
        {
            MenuItemType get() { return this->m_type; }
        }
    };
}
