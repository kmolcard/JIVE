#include "jive_GuiItem.h"

//======================================================================================================================
namespace jive
{
    //==================================================================================================================
    GuiItem::GuiItem(std::shared_ptr<juce::Component> comp, juce::ValueTree valueTree, GuiItem* parentItem)
        : tree{ valueTree }
        , component{ std::move(comp) }
        , parent{ parentItem }
        , name{ tree, "name" }
        , id{ tree, "id" }
        , visible{ tree, "visible", true }
        , alwaysOnTop{ tree, "always-on-top" }
        , bufferedToImage{ tree, "buffered-to-image" }
        , width{ tree, "width", -1 }
        , height{ tree, "height", -1 }
        , display{ tree, "display", Display::flex }
    {
        jassert(component != nullptr);

        name.onValueChange = [this]() {
            getComponent().setName(name);
            getComponent().setTitle(name);
        };
        getComponent().setName(name);
        getComponent().setTitle(name);

        id.onValueChange = [this]() {
            getComponent().setComponentID(id.get().toString());
        };
        getComponent().setComponentID(id.get().toString());

        visible.onValueChange = [this]() {
            getComponent().setVisible(visible);
        };
        getComponent().setVisible(visible);

        alwaysOnTop.onValueChange = [this]() {
            getComponent().setAlwaysOnTop(alwaysOnTop);
        };
        getComponent().setAlwaysOnTop(alwaysOnTop);

        bufferedToImage.onValueChange = [this]() {
            getComponent().setBufferedToImage(bufferedToImage);
        };
        getComponent().setBufferedToImage(bufferedToImage);

        width.onValueChange = [this]() {
            updateComponentSize();
        };
        height.onValueChange = [this]() {
            updateComponentSize();
        };
        updateComponentSize();

        component->addComponentListener(this);
        tree.addListener(this);
    }

    GuiItem::GuiItem(std::unique_ptr<juce::Component> comp, juce::ValueTree valueTree, GuiItem* parentItem)
        : GuiItem{ std::shared_ptr<juce::Component>{ std::move(comp) }, valueTree, parentItem }
    {
    }

    GuiItem::GuiItem(const GuiItem& other)
        : GuiItem{ other.component, other.tree, other.parent }
    {
    }

    GuiItem::~GuiItem()
    {
        component->removeComponentListener(this);
    }

    //==================================================================================================================
    void GuiItem::updateLayout()
    {
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

    void GuiItem::addChild(std::unique_ptr<GuiItem> child)
    {
        auto* newlyAddedChild = children.add(std::move(child));
        component->addChildComponent(newlyAddedChild->getComponent());
    }

    int GuiItem::getNumChildren() const
    {
        return children.size();
    }

    GuiItem& GuiItem::getChild(int index) const
    {
        jassert(index < children.size());
        return *children[index];
    }

    const GuiItem* GuiItem::getParent() const
    {
        return parent;
    }

    GuiItem* GuiItem::getParent()
    {
        return parent;
    }

    bool GuiItem::isTopLevel() const
    {
        return getParent() == nullptr;
    }

    bool GuiItem::isContainer() const
    {
        return true;
    }

    BoxModel GuiItem::getBoxModel() const
    {
        return boxModel;
    }

    juce::String GuiItem::getName() const
    {
        return name;
    }

    juce::Identifier GuiItem::getID() const
    {
        return id;
    }

    bool GuiItem::isVisible() const
    {
        return visible;
    }

    bool GuiItem::isAlwaysOnTop() const
    {
        return alwaysOnTop;
    }

    bool GuiItem::isBufferedToImage() const
    {
        return bufferedToImage;
    }

    float GuiItem::getWidth() const
    {
        return width;
    }

    float GuiItem::getHeight() const
    {
        return height;
    }

    GuiItem::Display GuiItem::getDisplay() const
    {
        return display;
    }

    bool GuiItem::hasAutoWidth() const
    {
        return static_cast<int>(width.get()) == -1;
    }

    bool GuiItem::hasAutoHeight() const
    {
        return static_cast<int>(height.get()) == -1;
    }

    //==================================================================================================================
    GuiItem::Iterator GuiItem::begin()
    {
        return Iterator{ *this, false };
    }

    const GuiItem::Iterator GuiItem::begin() const
    {
        return Iterator{ *this, false };
    }

    GuiItem::Iterator GuiItem::end()
    {
        return Iterator{ *this, true };
    }

    const GuiItem::Iterator GuiItem::end() const
    {
        return Iterator{ *this, true };
    }

    //==================================================================================================================
    void GuiItem::componentMovedOrResized(juce::Component& componentThatWasMovedOrResized,
                                          bool /*wasMoved*/,
                                          bool wasResized)
    {
        jassertquiet(&componentThatWasMovedOrResized == component.get());

        if (!wasResized)
            return;

        if (isTopLevel())
        {
            const auto bounds = getComponent().getLocalBounds();

            width = bounds.getWidth();
            height = bounds.getHeight();
        }

        updateLayout();
    }

    void GuiItem::componentVisibilityChanged(juce::Component& componentThatChangedVisiblity)
    {
        jassertquiet(&componentThatChangedVisiblity == component.get());

        visible = component->isVisible();
    }

    void GuiItem::componentNameChanged(juce::Component& componentThatChangedName)
    {
        jassertquiet(&componentThatChangedName == component.get());

        name = component->getName();
    }

    //==================================================================================================================
    void GuiItem::updateComponentSize()
    {
        const auto borderBounds = boxModel.getBorderBounds();
        component->setSize(juce::roundToInt(borderBounds.getWidth()), juce::roundToInt(borderBounds.getHeight()));
    }

    //==================================================================================================================
    GuiItem::Iterator::Iterator(const GuiItem& guiItem, bool isEnd)
        : item{ isEnd ? guiItem.children.end() : guiItem.children.begin() }
    {
    }

    GuiItem::Iterator& GuiItem::Iterator::operator++()
    {
        item++;
        return *this;
    }

    bool GuiItem::Iterator::operator==(const Iterator& other) const
    {
        return item == other.item;
    }

    bool GuiItem::Iterator::operator!=(const Iterator& other) const
    {
        return item != other.item;
    }

    GuiItem& GuiItem::Iterator::operator*()
    {
        return *(*item);
    }

    const GuiItem& GuiItem::Iterator::operator*() const
    {
        return *(*item);
    }
} // namespace jive
