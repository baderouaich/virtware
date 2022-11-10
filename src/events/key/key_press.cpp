#include "core/core.hpp"
#include "events/key/key_press.hpp"
#include "io/key/key.hpp"
#include <ostream>
#include <stdexcept>
using namespace Virtware;

KeyPressEvent::KeyPressEvent(const std::uint16_t keycode)
 : Event(EventType::Key),
   m_keycode(keycode)
{

}

std::uint16_t KeyPressEvent::get_keycode() const noexcept
{
    return m_keycode;
}

std::string KeyPressEvent::to_string() const
{
    if(const auto it = Key::KEYBOARD.find(m_keycode); 
       it != Key::KEYBOARD.end())
    {
        std::ostringstream oss{};
        oss << "key press " << it->second;
        return oss.str();
    }
    throw std::logic_error("Failed to convert KeyPressEvent to string due invalid keycode '"+ std::to_string(m_keycode) + '\'');
}

void KeyPressEvent::from_string(const std::string &str)
{
    // Prepare buffer to hold key string
    std::string keyname(32, '\000');
    // Scan string format to be parsed
    if (std::sscanf(str.c_str(), "key press %32s", keyname.data()) != 1)
        throw std::logic_error("Failed to parse KeyPressEvent from '" + str + '\'');

    // Validate key name
    const auto it = std::find_if(Key::KEYBOARD.begin(), Key::KEYBOARD.end(), [&keyname](const auto& key)
    {
        const auto& [keycode, valid_keyname] = key;
        return keyname == valid_keyname;
    });
    if(it != Key::KEYBOARD.end())
    {
        // Assign keycode with its correspondent value if valid
        m_keycode = it->first;
    }
    else 
        throw std::logic_error("Failed to parse KeyPressEvent. Invalid key name '"+keyname+"\'.");
}