/* Copyright (c) 2017 Hans-Kristian Arntzen
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "click_button.hpp"
#include "ui_manager.hpp"
#include "widget.hpp"

using namespace std;

namespace Granite
{
namespace UI
{
void ClickButton::set_text(std::string text)
{
	this->text = move(text);
	geometry_changed();
}

void ClickButton::reconfigure()
{
	auto &font = UIManager::get().get_font(FontSize::Small);
	vec2 minimum = font.get_text_geometry(text.c_str());
	geometry.minimum = minimum + 2.0f * geometry.margin;
}

void ClickButton::reconfigure_to_canvas(vec2, vec2)
{
}

Widget *ClickButton::on_mouse_button_pressed(vec2)
{
	click_held = true;
	return this;
}

void ClickButton::on_mouse_button_released(vec2)
{
	click_held = false;
}

float ClickButton::render(FlatRenderer &renderer, float layer, vec2 offset, vec2 size)
{
	auto &font = UIManager::get().get_font(FontSize::Small);
	renderer.render_text(font, text.c_str(), vec3(offset + geometry.margin, layer), size - 2.0f * geometry.margin,
	                     color * vec4(1.0f, 1.0f, 1.0f, click_held ? 0.25f : 1.0f), alignment);
	return layer;
}
}
}