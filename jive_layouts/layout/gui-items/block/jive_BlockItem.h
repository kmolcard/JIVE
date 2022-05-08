#pragma once

//======================================================================================================================
namespace jive
{
    //==================================================================================================================
    class BlockItem : public GuiItemDecorator
    {
    public:
        //==============================================================================================================
        explicit BlockItem(std::unique_ptr<GuiItem> itemToDecorate);

    private:
        //==============================================================================================================
        TypedValue<float> x;
        TypedValue<float> y;

        //==============================================================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BlockItem)
    };
} // namespace jive
