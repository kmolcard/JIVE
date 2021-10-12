#include <jive/jive.h>

//======================================================================================================================
class ViewRendererTests : public juce::UnitTest
{
public:
    //==================================================================================================================
    ViewRendererTests()
        : juce::UnitTest{ "jive::ViewRenderer" }
    {
    }

    //==================================================================================================================
    void runTest() final
    {
        testTreeWithToggleButtonType();
        testTreeWithTextButtonType();
        testTreeWithID();
        testNestedComponents();
        testCustomComponents();
        testDisplayOption();
    }

private:
    //==================================================================================================================
    void testTreeWithToggleButtonType()
    {
        beginTest("Rendering a view from a value tree with a type of 'ToggleButton' should return a "
                  "juce::ToggleButton");

        jive::ViewRenderer renderer;

        const auto& guiItem = renderer.renderView(juce::ValueTree{ "ToggleButton" });
        expect(dynamic_cast<const juce::ToggleButton*>(&guiItem.getComponent()) != nullptr);
    }

    void testTreeWithTextButtonType()
    {
        beginTest("Rendering a view from a value tree with a type of 'TextButton' should return a "
                  "juce::TextButton");

        jive::ViewRenderer renderer;

        const auto& guiItem = renderer.renderView(juce::ValueTree{ "TextButton" });
        expect(dynamic_cast<const juce::TextButton*>(&guiItem.getComponent()) != nullptr);
    }

    void testTreeWithID()
    {
        jive::ViewRenderer renderer;

        {
            beginTest("Rendering a view from a value tree with a type of 'ToggleButton' and a blank 'id' property "
                      "should return a component with no ID");

            juce::ValueTree tree{ "ToggleButton" };

            const auto& guiItem = renderer.renderView(tree);
            expectEquals(guiItem.getComponent().getComponentID(), juce::String{});
        }

        {
            beginTest("Rendering a view from a value tree with a type of 'ToggleButton' and an 'id' property with the "
                      "value '123' should return a component with the ID '123'");

            juce::ValueTree tree{ "ToggleButton" };
            tree.setProperty("id", "123", nullptr);

            const auto& guiItem = renderer.renderView(tree);
            expectEquals(guiItem.getComponent().getComponentID(), juce::String{ "123" });
        }

        {
            beginTest("Rendering a view from a value tree with a type of 'ToggleButton' and an 'id' property with the "
                      "value '567' should return a component with the ID '567'");

            juce::ValueTree tree{ "ToggleButton" };
            tree.setProperty("id", "567", nullptr);

            const auto& guiItem = renderer.renderView(tree);
            expectEquals(guiItem.getComponent().getComponentID(), juce::String{ "567" });
        }

        {
            beginTest("Rendering a view from a value tree with a type of 'TextButton' and an 'id' property with the "
                      "value '987' should return a component with the ID '987'");

            juce::ValueTree tree{ "TextButton" };
            tree.setProperty("id", "987", nullptr);

            const auto& guiItem = renderer.renderView(tree);
            expectEquals(guiItem.getComponent().getComponentID(), juce::String{ "987" });
        }

        {
            beginTest("Rendering a view from a value tree with a type of 'TextButton' and an 'id' property with the "
                      "value '543' should return a component with the ID '543'");

            juce::ValueTree tree{ "TextButton" };
            tree.setProperty("id", "543", nullptr);

            const auto& guiItem = renderer.renderView(tree);
            expectEquals(guiItem.getComponent().getComponentID(), juce::String{ "543" });
        }
    }

    void testNestedComponents()
    {
        jive::ViewRenderer renderer;

        {
            beginTest("Rendering a view from a valid value tree with no children should return a component with no "
                      "children");

            juce::ValueTree tree{ "TextButton" };

            const auto& guiItem = renderer.renderView(tree);
            expectEquals(guiItem.getComponent().getNumChildComponents(), 0);
        }

        {
            beginTest("Rendering a view from a value tree with a type of 'TextButton' and a single child node with a "
                      "type of 'ToggleButton' and and id of '123' should return a component with a single child with "
                      "an ID of '123'");

            juce::ValueTree tree{ "TextButton" };
            juce::ValueTree nestedTree{ "ToggleButton" };
            nestedTree.setProperty("id", "123", nullptr);
            tree.appendChild(nestedTree, nullptr);

            const auto& guiItem = renderer.renderView(tree);
            expectEquals(guiItem.getComponent().getNumChildComponents(), 1);
            expectEquals(guiItem.getComponent().getChildComponent(0)->getComponentID(), juce::String{ "123" });
        }

        {
            beginTest("Rendering a view from a value tree with a type of 'ToggleButton' and two child nodes each with "
                      "a type of 'TextButton', one with an id of '345' and the other with an id of '789' should return "
                      "a component with two children with the IDs '345' and '789'");

            juce::ValueTree tree{ "ToggleButton" };

            juce::ValueTree nestedTree1{ "TextButton" };
            nestedTree1.setProperty("id", "345", nullptr);
            tree.appendChild(nestedTree1, nullptr);

            juce::ValueTree nestedTree2{ "TextButton" };
            nestedTree2.setProperty("id", "789", nullptr);
            tree.appendChild(nestedTree2, nullptr);

            const auto& guiItem = renderer.renderView(tree);
            expectEquals(guiItem.getComponent().getNumChildComponents(), 2);
            expectEquals(guiItem.getComponent().getChildComponent(0)->getComponentID(), juce::String{ "345" });
            expectEquals(guiItem.getComponent().getChildComponent(1)->getComponentID(), juce::String{ "789" });
        }

        {
            beginTest("Rendering a view from a value tree with a type of 'ToggleButton' with a single child with a "
                      "type of 'TextButton' with an id of '285' and a single child with a type of 'ToggleButton' with "
                      "an id of '432' should return a component with a single child with an ID of '285' which has a "
                      "single child with an ID of '432'");

            juce::ValueTree tree{ "ToggleButton" };

            juce::ValueTree nestedTree1{ "TextButton" };
            nestedTree1.setProperty("id", "285", nullptr);
            tree.appendChild(nestedTree1, nullptr);

            juce::ValueTree nestedTree2{ "ToggleButton" };
            nestedTree2.setProperty("id", "432", nullptr);
            nestedTree1.appendChild(nestedTree2, nullptr);

            const auto& guiItem = renderer.renderView(tree);
            expectEquals(guiItem.getComponent().getNumChildComponents(), 1);
            expectEquals(guiItem.getComponent().getChildComponent(0)->getComponentID(), juce::String{ "285" });
            expectEquals(guiItem.getComponent().getChildComponent(0)->getNumChildComponents(), 1);
            expectEquals(guiItem.getComponent().getChildComponent(0)->getChildComponent(0)->getComponentID(), juce::String{ "432" });
        }
    }

    void testCustomComponents()
    {
        jive::ViewRenderer renderer;

        {
            beginTest("Rendering a view from a value tree with a type of 'MyCustomComponent' after giving the view "
                      "renderer a creator for components from trees with the type 'MyCustomComponent' returns a "
                      "component with the custom type");

            struct MyCustomComponent : public juce::Component {};

            renderer.setComponentCreator("MyCustomComponent", []() { return std::make_unique<MyCustomComponent>(); });

            juce::ValueTree tree{ "MyCustomComponent" };

            const auto& guiItem = renderer.renderView(tree);
            expect(dynamic_cast<const MyCustomComponent*>(&guiItem.getComponent()) != nullptr);
        }

        {
            beginTest("Rendering a view from a value tree with a type of 'ToggleButton' after overriding the "
                      "renderer's default creator for toggle buttons to one that returns a custom component should "
                      "return a component with the custom type");

            struct NotAToggleButton : public juce::Component {};

            renderer.setComponentCreator("ToggleButton", []() { return std::make_unique<NotAToggleButton>(); });

            juce::ValueTree tree{ "ToggleButton" };

            const auto& guiItem = renderer.renderView(tree);
            expect(dynamic_cast<const NotAToggleButton*>(&guiItem.getComponent()) != nullptr);
        }

        {
            beginTest("Rendering a view from a value tree with a type of 'ToggleButton' after overriding the "
                      "renderer's default creator for toggle buttons to one that returns a custom component and then "
                      "resetting the renderer to use its default creators should return a juce::ToggleButton");

            struct NotAToggleButton : public juce::Component {};
            renderer.setComponentCreator("ToggleButton", []() { return std::make_unique<NotAToggleButton>(); });
            renderer.resetComponentCreators();

            juce::ValueTree tree{ "ToggleButton" };

            const auto& guiItem = renderer.renderView(tree);
            expect(dynamic_cast<const juce::ToggleButton*>(&guiItem.getComponent()) != nullptr);
        }
    }

    void testDisplayOption()
    {
        jive::ViewRenderer renderer;

        {
            beginTest("Rendering a view from a value tree with no properties set returns a GUI item with its display "
                      "property set to 'flex'");

            juce::ValueTree tree{ "ToggleButton" };
            tree.setProperty("display",
                             juce::VariantConverter<jive::GuiItem::Display>::toVar (jive::GuiItem::Display::flex),
                             nullptr);

            const auto& guiItem = renderer.renderView(tree);
            expect(guiItem.getDisplay() == jive::GuiItem::Display::flex);
        }

        {
            beginTest("Rendering a view from a value tree with its 'display' property set to 'grid' returns a GUI item "
                      "with its display property set to 'grid'");

            juce::ValueTree tree{ "TextButton" };
            tree.setProperty("display",
                             juce::VariantConverter<jive::GuiItem::Display>::toVar (jive::GuiItem::Display::grid),
                             nullptr);

            const auto& guiItem = renderer.renderView(tree);
            expect(guiItem.getDisplay() == jive::GuiItem::Display::grid);
        }
    }
};

//======================================================================================================================
static ViewRendererTests viewRendererTests;