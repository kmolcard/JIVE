#include "jive_GuiItem.h"

//======================================================================================================================
namespace jive
{
    //==================================================================================================================
    GuiItem::GuiItem(std::unique_ptr<juce::Component> comp, juce::ValueTree valueTree)
        : component{ std::move(comp) }
        , tree{ valueTree }
        , width{ tree, "width", nullptr, -1 }
        , height{ tree, "height", nullptr, -1 }
        , display{ tree, "display", nullptr, Display::flex }
        , flexDirection{ tree, "flex-direction", nullptr, juce::FlexBox::Direction::column }
        , flexWrap{ tree, "flex-wrap", nullptr }
    {
        jassert(component != nullptr);

        componentIdChanged();

        component->addComponentListener(this);
        tree.addListener(this);
    }

    //==================================================================================================================
    void GuiItem::addChild(std::unique_ptr<GuiItem> child)
    {
        auto* newlyAddedChild = children.add(std::move(child));
        component->addAndMakeVisible(newlyAddedChild->getComponent());
    }

    //==================================================================================================================
    juce::Component& GuiItem::getComponent()
    {
        return *component;
    }

    const juce::Component& GuiItem::getComponent() const
    {
        return *component;
    }

    int GuiItem::getNumChildren() const
    {
        return children.size();
    }

    GuiItem& GuiItem::getChild(int index)
    {
        jassert(index < children.size());
        return *children[index];
    }

    GuiItem::Display GuiItem::getDisplay() const
    {
        return display;
    }

    //==================================================================================================================
    GuiItem::operator juce::FlexBox()
    {
        juce::FlexBox flex;
        flex.flexDirection = flexDirection;
        flex.flexWrap = flexWrap;

        for (auto* child : children)
            flex.items.add(*child);

        return flex;
    }

    GuiItem::operator juce::FlexItem()
    {
        juce::FlexItem item{ *component };

        item.width = static_cast<float>(width);
        item.height = static_cast<float>(height);

        return item;
    }

    //==================================================================================================================
    void GuiItem::valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyChanged,
                                           const juce::Identifier& propertyID)
    {
        jassert(treeWhosePropertyChanged == tree);

        if (propertyID == juce::Identifier{ "id" })
            componentIdChanged();
    }

    void GuiItem::componentMovedOrResized(juce::Component& componentThatWasMovedOrResized,
                                          bool /*wasMoved*/,
                                          bool wasResized)
    {
        jassert(&componentThatWasMovedOrResized == component.get());

        if (! wasResized)
            return;

        updateLayout();
    }

    //==================================================================================================================
    void GuiItem::componentIdChanged()
    {
        component->setComponentID(tree["id"]);
    }

    void performFlexLayout(GuiItem& item)
    {
        auto flex = static_cast<juce::FlexBox>(item);
        flex.performLayout(item.getComponent().getLocalBounds());
    }

    void GuiItem::updateLayout()
    {
        switch (display)
        {
            case Display::flex:
                performFlexLayout(*this);
                break;
            default:
                // Unhandled display type
                jassertfalse;
        }
    }
} // namespace jive
