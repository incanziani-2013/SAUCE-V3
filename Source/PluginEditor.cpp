#include "PluginEditor.h"

namespace {
const juce::Colour kBg1  (0xff1b0d2b), kBg2 (0xff07060b);
const juce::Colour kViolet(0xffa855f7), kPink(0xffec4899), kLilac(0xffc4b5fd);
const juce::Colour kText (0xfff8f7ff), kDim (0xff8b7aa8), kPanel(0xff160d24);

juce::Font F(float h, int style = juce::Font::plain) { return juce::Font(juce::FontOptions(h, style)); }

void styleSlider(juce::Slider& s)
{
    s.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    s.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 76, 20);
    s.setRotaryParameters(juce::MathConstants<float>::pi * 1.25f,
                          juce::MathConstants<float>::pi * 2.75f, true);
    s.setColour(juce::Slider::textBoxTextColourId, kText);
    s.setColour(juce::Slider::textBoxBackgroundColourId, kPanel);
    s.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
}
}

// ---------------------------------------------------------------- LookAndFeel
SauceLookAndFeel::SauceLookAndFeel()
{
    setColour(juce::PopupMenu::backgroundColourId, kPanel);
    setColour(juce::PopupMenu::textColourId, kText);
    setColour(juce::PopupMenu::highlightedBackgroundColourId, juce::Colour(0xff6d28a8));
}

void SauceLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int w, int h, float pos,
                                        float a0, float a1, juce::Slider& s)
{
    const bool big = w > 200;
    auto b = juce::Rectangle<float>((float)x, (float)y, (float)w, (float)h).reduced(big ? 14.0f : 6.0f);
    const float r = juce::jmin(b.getWidth(), b.getHeight()) * 0.5f;
    const auto c = b.getCentre();
    const float ang = a0 + pos * (a1 - a0);
    const float th = big ? 12.0f : 5.0f;

    if (big) { // glow que respira con la señal
        const float gl = 0.18f + 0.55f * glow + 0.25f * pos;
        g.setGradientFill(juce::ColourGradient(kViolet.withAlpha(gl), c.x, c.y,
                          kViolet.withAlpha(0.0f), c.x, c.y - r * 1.25f, true));
        g.fillEllipse(c.x - r * 1.25f, c.y - r * 1.25f, r * 2.5f, r * 2.5f);
        // ticks
        for (int i = 0; i <= 30; ++i) {
            const float t = a0 + (a1 - a0) * i / 30.0f, s0 = std::sin(t), c0 = std::cos(t);
            const bool on = (float)i / 30.0f <= pos;
            g.setColour(on ? kLilac.withAlpha(0.9f) : juce::Colour(0xff342343));
            const float r1 = r + 4.0f, r2 = r + (i % 5 == 0 ? 12.0f : 8.0f);
            g.drawLine(c.x + r1 * s0, c.y - r1 * c0, c.x + r2 * s0, c.y - r2 * c0, i % 5 == 0 ? 2.0f : 1.2f);
        }
    }
    const float ar = r - th * 0.5f - (big ? 2.0f : 0.0f);
    juce::Path track; track.addCentredArc(c.x, c.y, ar, ar, 0, a0, a1, true);
    g.setColour(juce::Colour(0xff2a1a3d));
    g.strokePath(track, juce::PathStrokeType(th, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    juce::Path val; val.addCentredArc(c.x, c.y, ar, ar, 0, a0, juce::jmax(a0 + 0.001f, ang), true);
    g.setGradientFill(juce::ColourGradient(kViolet, c.x - ar, c.y, kPink, c.x + ar, c.y, false));
    g.strokePath(val, juce::PathStrokeType(th, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    // cuerpo
    const float br = ar - th * 0.5f - (big ? 12.0f : 5.0f);
    g.setGradientFill(juce::ColourGradient(juce::Colour(0xff3a2257), c.x, c.y - br, juce::Colour(0xff0d0716), c.x, c.y + br, false));
    g.fillEllipse(c.x - br, c.y - br, br * 2, br * 2);
    g.setColour(kViolet.withAlpha(0.55f)); g.drawEllipse(c.x - br, c.y - br, br * 2, br * 2, big ? 2.0f : 1.2f);
    g.setColour(juce::Colours::white.withAlpha(0.07f));
    g.fillEllipse(c.x - br * 0.7f, c.y - br * 0.92f, br * 1.4f, br * 0.75f);

    // puntero
    const float s0 = std::sin(ang), c0 = std::cos(ang);
    const float p1 = br * 0.55f, p2 = br * 0.9f;
    g.setColour(kText);
    g.drawLine(c.x + p1 * s0, c.y - p1 * c0, c.x + p2 * s0, c.y - p2 * c0, big ? 5.0f : 3.0f);
    juce::ignoreUnused(s);
}

void SauceLookAndFeel::drawButtonBackground(juce::Graphics& g, juce::Button& b, const juce::Colour&, bool hi, bool down)
{
    auto r = b.getLocalBounds().toFloat().reduced(0.5f);
    g.setColour(down ? juce::Colour(0xff6d28a8) : hi ? juce::Colour(0xff3a2057) : juce::Colour(0xff281638));
    g.fillRoundedRectangle(r, r.getHeight() * 0.5f);
    g.setColour(kViolet.withAlpha(0.45f)); g.drawRoundedRectangle(r, r.getHeight() * 0.5f, 1.0f);
}

void SauceLookAndFeel::drawToggleButton(juce::Graphics& g, juce::ToggleButton& b, bool hi, bool)
{
    auto r = b.getLocalBounds().toFloat().reduced(0.5f);
    const bool on = b.getToggleState();
    if (on) g.setGradientFill(juce::ColourGradient(juce::Colour(0xff7c3aed), r.getX(), 0, kPink.darker(0.2f), r.getRight(), 0, false));
    else    g.setColour(hi ? juce::Colour(0xff3a2057) : juce::Colour(0xff281638));
    g.fillRoundedRectangle(r, r.getHeight() * 0.5f);
    g.setColour(kViolet.withAlpha(on ? 0.9f : 0.45f)); g.drawRoundedRectangle(r, r.getHeight() * 0.5f, 1.0f);
    g.setColour(on ? kText : kLilac); g.setFont(F(12.5f, juce::Font::bold));
    g.drawText(b.getButtonText(), b.getLocalBounds(), juce::Justification::centred);
}

void SauceLookAndFeel::drawComboBox(juce::Graphics& g, int w, int h, bool, int, int, int, int, juce::ComboBox& box)
{
    auto r = juce::Rectangle<float>(0.5f, 0.5f, w - 1.0f, h - 1.0f);
    g.setColour(kPanel); g.fillRoundedRectangle(r, h * 0.5f);
    g.setColour(juce::Colour(0xff6d28a8)); g.drawRoundedRectangle(r, h * 0.5f, 1.2f);
    juce::Path arrow; const float ax = w - 20.0f, ay = h * 0.5f;
    arrow.addTriangle(ax, ay - 2.5f, ax + 9, ay - 2.5f, ax + 4.5f, ay + 3.5f);
    g.setColour(box.findColour(juce::ComboBox::textColourId)); g.fillPath(arrow);
}

// --------------------------------------------------------------------- Editor
SauceAudioProcessorEditor::SauceAudioProcessorEditor(SauceAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p),
      sauceAtt(p.parameters,"sauce",sauce), inputAtt(p.parameters,"input",input),
      outputAtt(p.parameters,"output",output), mixAtt(p.parameters,"mix",mix),
      subAtt(p.parameters,"sub",sub), transientAtt(p.parameters,"transient",transient),
      boostAtt(p.parameters,"boost",boost), osAtt(p.parameters,"oversampling",oversampling),
      enabledAtt(p.parameters,"enabled",enabled), autoGainAtt(p.parameters,"autogain",autoGain)
{
    setLookAndFeel(&lnf);
    setSize(720, baseH);
    for (auto* s : {&sauce,&input,&output,&mix,&sub,&transient,&boost}) styleSlider(*s);
    sauce.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    sauce.setTextValueSuffix("%");
    for (auto* s : {&input,&output}) s->setTextValueSuffix(" dB");
    mix.setTextValueSuffix("%"); sub.setTextValueSuffix("%"); transient.setTextValueSuffix("%");
    boost.setTextValueSuffix(" dB");
    for (auto* c : {&preset,&oversampling}) {
        c->setColour(juce::ComboBox::textColourId, kText);
        addAndMakeVisible(*c);
    }
    for (int i=0;i<8;++i) preset.addItem(processor.getProgramName(i),i+1);
    preset.setSelectedId(1);
    preset.onChange = [this] { processor.setPreset(preset.getSelectedId()-1); };
    oversampling.addItemList({"1x","2x","4x","8x"},1);
    for (auto* b : {(juce::Button*)&advanced,(juce::Button*)&enabled,(juce::Button*)&autoGain,
                    (juce::Button*)&prev,(juce::Button*)&next,(juce::Button*)&abButton}) {
        b->setColour(juce::TextButton::textColourOffId, kText);
        addAndMakeVisible(*b);
    }
    advanced.onClick = [this] { showAdvanced = advanced.getToggleState(); setSize(720, showAdvanced ? advH : baseH); };
    prev.onClick = [this]{ preset.setSelectedId(juce::jmax(1, preset.getSelectedId()-1)); };
    next.onClick = [this]{ preset.setSelectedId(juce::jmin(8, preset.getSelectedId()+1)); };
    abButton.onClick = [this]{ processor.toggleAB(); abButton.setButtonText(processor.isB() ? "B" : "A/B"); };
    for (auto* c : {&title,&percent,&inputLabel,&outputLabel}) {
        c->setColour(juce::Label::textColourId, kText);
        c->setInterceptsMouseClicks(false, false);
        addAndMakeVisible(*c);
    }
    title.setText("SAUCE", juce::dontSendNotification);
    title.setFont(F(30.0f, juce::Font::bold));
    percent.setJustificationType(juce::Justification::centred);
    percent.setFont(F(46.0f, juce::Font::bold));
    for (auto* s : {&sauce,&input,&output,&mix,&sub,&transient,&boost}) addAndMakeVisible(*s);
    percent.toFront(false); // el % se dibuja encima de la perilla; no intercepta el mouse
    startTimerHz(30);
}

SauceAudioProcessorEditor::~SauceAudioProcessorEditor() { setLookAndFeel(nullptr); }

void SauceAudioProcessorEditor::drawVMeter(juce::Graphics& g, juce::Rectangle<float> r, float level, const juce::String& name)
{
    constexpr int n = 24;
    const float sh = r.getHeight() / n;
    for (int i = 0; i < n; ++i) {
        const float f = (float)i / (n - 1);
        const bool on = level > (float)i / n;
        auto col = f > 0.85f ? kPink : f > 0.6f ? kViolet : juce::Colour(0xff7c3aed);
        g.setColour(on ? col : juce::Colour(0xff241532));
        g.fillRoundedRectangle(r.getX(), r.getBottom() - (i + 1) * sh + 1.5f, r.getWidth(), sh - 3.0f, 2.0f);
    }
    g.setColour(kDim); g.setFont(F(11.0f, juce::Font::bold));
    g.drawText(name, (int)r.getX() - 10, (int)r.getBottom() + 6, (int)r.getWidth() + 20, 14, juce::Justification::centred);
}

void SauceAudioProcessorEditor::drawCup(juce::Graphics& g, juce::Rectangle<float> r)
{
    const float level = 0.30f + 0.58f * (float)sauce.getValue() / 100.0f;
    auto cup = r;
    const float inset = cup.getWidth() * 0.16f;
    juce::Path body;
    body.startNewSubPath(cup.getX(), cup.getY());
    body.lineTo(cup.getRight(), cup.getY());
    body.lineTo(cup.getRight() - inset, cup.getBottom());
    body.lineTo(cup.getX() + inset, cup.getBottom());
    body.closeSubPath();
    body = body.createPathWithRoundedCorners(5.0f);

    g.setColour(juce::Colour(0x227c3aed)); g.fillPath(body);
    g.saveState();
    g.reduceClipRegion(body); // clip real al vaso
    const float top = cup.getBottom() - cup.getHeight() * level;
    const float amp = 2.5f + 2.0f * (float)sauce.getValue() / 100.0f;
    juce::Path liquid;
    liquid.startNewSubPath(cup.getX() - 2, cup.getBottom() + 2);
    for (float x = cup.getX() - 2; x <= cup.getRight() + 2; x += 3.0f)
        liquid.lineTo(x, top + std::sin(x * 0.09f + phase * 2.0f) * amp);
    liquid.lineTo(cup.getRight() + 2, cup.getBottom() + 2); liquid.closeSubPath();
    g.setGradientFill(juce::ColourGradient(juce::Colour(0xffb36bff), 0, top, juce::Colour(0xff4c1d95), 0, cup.getBottom(), false));
    g.fillPath(liquid);
    // burbujas
    const float lh = cup.getBottom() - top;
    for (int i = 0; i < 6; ++i) {
        const float t = std::fmod(phase * (10.0f + i * 3.0f) + i * 31.0f, juce::jmax(10.0f, lh));
        const float bx = cup.getX() + cup.getWidth() * (0.25f + 0.1f * i) + std::sin(phase * 2 + i) * 3.0f;
        g.setColour(kLilac.withAlpha(0.35f));
        g.drawEllipse(bx, cup.getBottom() - 6 - t, 4.0f + (i % 3), 4.0f + (i % 3), 1.0f);
    }
    // hielos flotando
    for (int i = 0; i < 3; ++i) {
        const float cx = cup.getX() + cup.getWidth() * (0.30f + i * 0.2f);
        const float cy = top + 10.0f + (i % 2) * 14.0f + std::sin(phase * 1.4f + i * 2.1f) * 3.5f;
        juce::Path ice; ice.addRoundedRectangle(-9.0f, -9.0f, 18.0f, 18.0f, 3.5f);
        ice.applyTransform(juce::AffineTransform::rotation(std::sin(phase + i) * 0.35f + i * 0.3f).translated(cx, cy));
        g.setColour(juce::Colours::white.withAlpha(0.28f)); g.fillPath(ice);
        g.setColour(kLilac.withAlpha(0.8f)); g.strokePath(ice, juce::PathStrokeType(1.2f));
    }
    g.restoreState();

    g.setColour(kLilac.withAlpha(0.85f)); g.strokePath(body, juce::PathStrokeType(2.0f));
    g.setColour(juce::Colours::white.withAlpha(0.25f)); // brillo del vidrio
    g.drawLine(cup.getX() + 9, cup.getY() + 14, cup.getX() + 9 + inset * 0.35f, cup.getBottom() - 16, 2.0f);
    g.setColour(kLilac); g.fillRoundedRectangle(cup.getX() - 5, cup.getY() - 6, cup.getWidth() + 10, 6.0f, 3.0f); // tapa
}

void SauceAudioProcessorEditor::paint(juce::Graphics& g)
{
    const auto b = getLocalBounds().toFloat();
    g.setGradientFill(juce::ColourGradient(kBg1, 0, 0, kBg2, b.getWidth(), (float)baseH, false));
    g.fillAll();
    // halo detrás de la perilla
    g.setGradientFill(juce::ColourGradient(kViolet.withAlpha(0.14f), 360, 260, kViolet.withAlpha(0.0f), 360, 520, true));
    g.fillRect(b);
    g.setColour(kViolet.withAlpha(0.7f)); g.drawRoundedRectangle(b.reduced(6), 18, 1.2f);

    // header
    g.setColour(kDim); g.setFont(F(11.5f, juce::Font::bold));
    g.drawText("CLIPPER 808  ·  V1 PRO  ·  prod.shiny", 34, 50, 300, 16, juce::Justification::left);
    g.setColour(kViolet.withAlpha(0.25f)); g.drawLine(24, 100, 696, 100, 1.0f);

    // medidores y vaso
    drawVMeter(g, { 56, 140, 16, 240 }, smoothIn, "IN");
    drawVMeter(g, { 106, 140, 16, 240 }, smoothOut, "OUT");
    drawCup(g, { 556, 150, 112, 175 });
    g.setColour(kDim); g.setFont(F(11.0f, juce::Font::bold));
    g.drawText("LEAN WITH ICE", 530, 340, 164, 16, juce::Justification::centred);
    g.setColour(kLilac); g.setFont(F(12.0f, juce::Font::bold));
    g.drawText("SAUCE INTENSITY", 210, 420, 300, 18, juce::Justification::centred);

    if (showAdvanced) {
        g.setColour(kPanel.withAlpha(0.85f)); g.fillRoundedRectangle(24, 456, 672, 176, 14);
        g.setColour(kViolet.withAlpha(0.35f)); g.drawRoundedRectangle(24, 456, 672, 176, 14, 1.0f);
        g.setColour(kDim); g.setFont(F(11.0f, juce::Font::bold));
        const char* names[] = { "INPUT","OUTPUT","MIX","SUB","TRANSIENT","BOOST" };
        juce::Slider* sl[] = { &input,&output,&mix,&sub,&transient,&boost };
        for (int i = 0; i < 6; ++i)
            g.drawText(names[i], sl[i]->getX(), sl[i]->getY() - 2, sl[i]->getWidth(), 14, juce::Justification::centred);
        g.drawText("OVERSAMPLING", 170, 600, 100, 24, juce::Justification::centredRight);
    }
}

void SauceAudioProcessorEditor::resized()
{
    title.setBounds(30, 16, 200, 36);
    prev.setBounds(432, 22, 30, 28); preset.setBounds(466, 22, 188, 28); next.setBounds(658, 22, 30, 28);
    advanced.setBounds(432, 62, 112, 28); enabled.setBounds(552, 62, 60, 28); abButton.setBounds(620, 62, 68, 28);
    sauce.setBounds(210, 110, 300, 300);
    percent.setBounds(285, 225, 150, 56);
    inputLabel.setBounds(0, 0, 0, 0); outputLabel.setBounds(0, 0, 0, 0);
    if (showAdvanced) {
        juce::Slider* sl[] = { &input,&output,&mix,&sub,&transient,&boost };
        for (int i = 0; i < 6; ++i) sl[i]->setBounds(36 + i * 110, 472, 104, 118);
        autoGain.setBounds(36, 600, 120, 24); oversampling.setBounds(278, 600, 90, 24);
    } else {
        for (juce::Component* c : std::initializer_list<juce::Component*>{&input,&output,&mix,&sub,&transient,&boost,&autoGain,&oversampling})
            c->setBounds(0,0,0,0);
    }
}

void SauceAudioProcessorEditor::timerCallback()
{
    phase += 0.045f;
    const float ip = juce::jlimit(0.0f, 1.0f, processor.getInputPeak());
    const float op = juce::jlimit(0.0f, 1.0f, processor.getOutputPeak());
    smoothIn  = juce::jmax(ip, smoothIn  * 0.86f);
    smoothOut = juce::jmax(op, smoothOut * 0.86f);
    lnf.glow  = smoothOut;
    percent.setText(juce::String((int)std::round(sauce.getValue())) + "%", juce::dontSendNotification);
    repaint();
}
