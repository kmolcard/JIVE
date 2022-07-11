#pragma once

//======================================================================================================================
namespace jive
{
    //==================================================================================================================
    class Spinner : public Slider
    {
    public:
        //==============================================================================================================
        explicit Spinner(std::unique_ptr<GuiItem> itemToDecorate);

        //==============================================================================================================
        float getWidth() const override;
        float getHeight() const override;

    private:
        //==============================================================================================================
        juce::Slider::SliderStyle getStyleForOrientation(Orientation orientation) final;

        //==============================================================================================================
        TypedValue<bool> draggable;

        //==============================================================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Spinner)
    };
} // namespace jive
