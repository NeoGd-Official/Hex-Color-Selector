#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/GJGarageLayer.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <cmath>

using namespace geode::prelude;

static cocos2d::ccColor3B getRainbowColor(float hue) {
    hue = std::fmod(hue, 1.0f);

    if (hue < 0.0f)
        hue += 1.0f;

    float h = hue * 6.0f;
    int sector = static_cast<int>(std::floor(h));
    float fraction = h - sector;

    float q = 1.0f - fraction;
    float t = fraction;

    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;

    switch (sector % 6) {
        case 0:
            r = 1.0f;
            g = t;
            b = 0.0f;
            break;

        case 1:
            r = q;
            g = 1.0f;
            b = 0.0f;
            break;

        case 2:
            r = 0.0f;
            g = 1.0f;
            b = t;
            break;

        case 3:
            r = 0.0f;
            g = q;
            b = 1.0f;
            break;

        case 4:
            r = t;
            g = 0.0f;
            b = 1.0f;
            break;

        case 5:
            r = 1.0f;
            g = 0.0f;
            b = q;
            break;
    }

    return cocos2d::ccc3(
        static_cast<GLubyte>(r * 255.0f),
        static_cast<GLubyte>(g * 255.0f),
        static_cast<GLubyte>(b * 255.0f)
    );
}


// ============================================================
// EXISTING HEX COLOR / RAINBOW PLAYER CODE
// ============================================================

class $modify(HexColorPlayer, PlayerObject) {
    struct Fields {
        float rainbowTime = 0.0f;
    };

    void update(float dt) {
        PlayerObject::update(dt);

        auto mod = Mod::get();

        if (!mod->getSettingValue<bool>("enabled"))
            return;

        float speed = mod->getSettingValue<float>("rainbow-speed");

        m_fields->rainbowTime += dt * speed;

        if (m_fields->rainbowTime > 100000.0f) {
            m_fields->rainbowTime = std::fmod(
                m_fields->rainbowTime,
                1.0f
            );
        }

        auto color1 =
            mod->getSettingValue<cocos2d::ccColor3B>("color1");

        auto color2 =
            mod->getSettingValue<cocos2d::ccColor3B>("color2");

        auto glow =
            mod->getSettingValue<cocos2d::ccColor3B>("glow");

        auto rainbow =
            getRainbowColor(
                std::fmod(m_fields->rainbowTime, 1.0f)
            );

        if (mod->getSettingValue<bool>("rainbow-color1"))
            color1 = rainbow;

        if (mod->getSettingValue<bool>("rainbow-color2"))
            color2 = rainbow;

        if (mod->getSettingValue<bool>("rainbow-glow"))
            glow = rainbow;

        m_playerColor1 = color1;
        m_playerColor2 = color2;

        if (m_iconSprite)
            m_iconSprite->setColor(color1);

        if (m_iconSpriteSecondary)
            m_iconSpriteSecondary->setColor(color2);

        if (m_vehicleSprite)
            m_vehicleSprite->setColor(color1);

        if (m_vehicleSpriteSecondary)
            m_vehicleSpriteSecondary->setColor(color2);

        if (m_iconGlow)
            m_iconGlow->setColor(glow);

        if (m_vehicleGlow)
            m_vehicleGlow->setColor(glow);
    }
};


// ============================================================
// NEW WRENCH BUTTON
// ============================================================
//
// Adds your wrench image to the Geometry Dash icon/garage screen.
// Pressing it opens the Hex Color Selector settings.
//
// The image file needs to be:
// resources/hex-button.png
//
// ============================================================

class $modify(HexColorGarage, GJGarageLayer) {

    bool init() {
        if (!GJGarageLayer::init())
            return false;

        auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();

        // Create the menu that will contain our button.
        auto menu = cocos2d::CCMenu::create();

        if (!menu)
            return true;

        menu->setPosition({0, 0});
        menu->setID("hex-color-selector-menu");

        // Load the wrench image from this mod's resources.
        auto sprite = cocos2d::CCSprite::create("hex-button.png"_spr);

        if (!sprite)
            return true;

        // The original image is large, so scale it down for the UI.
        sprite->setScale(0.18f);

        // Create the actual clickable button.
        auto button = CCMenuItemSpriteExtra::create(
            sprite,
            this,
            menu_selector(HexColorGarage::onHexColorButton)
        );

        if (!button)
            return true;

        button->setID("hex-color-selector-button");

        // Position the button near the top-right of the garage.
        button->setPosition({
            winSize.width - 45.0f,
            winSize.height - 45.0f
        });

        menu->addChild(button);

        // Add the menu to the garage layer.
        this->addChild(menu, 100);

        return true;
    }

    void onHexColorButton(cocos2d::CCObject*) {
        // Open this mod's existing Geode settings.
        geode::openSettingsPopup(Mod::get());
    }
};
