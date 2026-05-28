#include "TypeSelectPanel.h"
#include "Scanner.h"
#include "UIContext.h"


TypeSelectPanel::TypeSelectPanel(UIContext& ui, Scanner& sc) 
	:ui(ui),sc(sc)
{
}
void TypeSelectPanel::draw()
{
	const char* types[] = { "int32", "int64", "float" };

	ImGui::Combo("Type", &ui.selectedType, types, IM_ARRAYSIZE(types));

}
