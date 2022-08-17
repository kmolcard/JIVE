#include <jive_layouts/jive_layouts.h>

//======================================================================================================================
namespace jive
{
    //==================================================================================================================
    Spinner::Spinner(std::unique_ptr<GuiItem> itemToDecorate)
        : Slider{ std::move(itemToDecorate) }
        , draggable{ tree, "draggable" }
    {
        draggable.onValueChange = [this]() {
            getSlider().setIncDecButtonsMode(draggable ? juce::Slider::incDecButtonsDraggable_AutoDirection : juce::Slider::incDecButtonsNotDraggable);
        };
        getSlider().setIncDecButtonsMode(draggable ? juce::Slider::incDecButtonsDraggable_AutoDirection : juce::Slider::incDecButtonsNotDraggable);

        updateStyle();
    }

    //==================================================================================================================
    float Spinner::getWidth() const
    {
        if (!hasAutoWidth())
            return Slider::getWidth();

        return 50.f;
    }

    float Spinner::getHeight() const
    {
        if (!hasAutoHeight())
            return Slider::getHeight();

        return 20.f;
    }

    //==================================================================================================================
    juce::Slider::SliderStyle Spinner::getStyleForOrientation(Orientation /* orientation */)
    {
        return juce::Slider::IncDecButtons;
    }
} // namespace jive

//======================================================================================================================
#if JIVE_UNIT_TESTS
class SpinnerTest : public juce::UnitTest
{
public:
    SpinnerTest()
        : juce::UnitTest{ "jive::Spinner", "jive" }
    {
    }

    void runTest() final
    {
        testOrientation();
        testAutoSize();
    }

private:
    std::unique_ptr<jive::Spinner> createSpinner(juce::ValueTree tree)
    {
        jive::Interpreter interpreter;

        return std::make_unique<jive::Spinner>(interpreter.interpret(tree));
    }

    void testOrientation()
    {
        beginTest("orientation");

        juce::ValueTree tree{
            "Spinner",
            {
                { "orientation", "horizontal" },
            },
        };
        auto item = createSpinner(tree);
        expectEquals(item->getSlider().getSliderStyle(), juce::Slider::IncDecButtons);

        tree.setProperty("orientation", "vertical", nullptr);
        expectEquals(item->getSlider().getSliderStyle(), juce::Slider::IncDecButtons);
    }

    void testAutoSize()
    {
        beginTest("auto size");

        juce::ValueTree tree{ "Spinner" };
        auto item = createSpinner(tree);
        expectEquals(item->getWidth(), 50.0f);
        expectEquals(item->getHeight(), 20.0f);

        tree.setProperty("width", 38.0f, nullptr);
        expectEquals(item->getWidth(), 38.0f);

        tree.setProperty("height", 73.0f, nullptr);
        expectEquals(item->getHeight(), 73.0f);
    }
};

static SpinnerTest spinnerTest;
#endif