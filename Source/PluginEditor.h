#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

// Look & feel propio: perilla gigante con glow, perillas chicas, botones tipo pill
class SauceLookAndFeel : public juce::LookAndFeel_V4
{
public:
    SauceLookAndFeel();
    void drawRotarySlider(juce::Graphics&, int x, int y, int w, int h, float pos,
                          float startAngle, float endAngle, juce::Slider&) override;
    void drawToggleButton(juce::Graphics&, juce::ToggleButton&, bool highlighted, bool down) override;
    void drawButtonBackground(juce::Graphics&, juce::Button&, const juce::Colour&, bool highlighted, bool down) override;
    void drawComboBox(juce::Graphics&, int w, int h, bool isDown, int, int, int, int, juce::ComboBox&) override;
    float glow = 0.0f; // 0..1, reacciona al nivel de salida
};

class SauceAudioProcessorEditor : public juce::AudioProcessorEditor, private juce::Timer
{
public:
    explicit SauceAudioProcessorEditor(SauceAudioProcessor&);
    ~SauceAudioProcessorEditor() override;
    void paint(juce::Graphics&) override;
    void resized() override;
private:
    void timerCallback() override;
    void drawCup(juce::Graphics&, juce::Rectangle<float>);
    void drawVMeter(juce::Graphics&, juce::Rectangle<float>, float level, const juce::String& name);

    SauceLookAndFeel lnf;
    SauceAudioProcessor& processor;
    juce::Slider sauce, input, output, mix, sub, transient, boost;
    juce::ComboBox preset, oversampling;
    juce::ToggleButton advanced {"ADVANCED"}, enabled {"ON"}, autoGain {"AUTO GAIN"};
    juce::TextButton prev {"<"}, next {">"}, abButton {"A/B"};
    juce::Label title, percent, inputLabel, outputLabel;
    juce::AudioProcessorValueTreeState::SliderAttachment
        sauceAtt, inputAtt, outputAtt, mixAtt, subAtt, transientAtt, boostAtt;
    juce::AudioProcessorValueTreeState::ComboBoxAttachment osAtt;
    juce::AudioProcessorValueTreeState::ButtonAttachment enabledAtt, autoGainAtt;
    bool showAdvanced = false;
    float phase = 0.0f, smoothIn = 0.0f, smoothOut = 0.0f;
    static constexpr int baseH = 480, advH = 648;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SauceAudioProcessorEditor)
};
