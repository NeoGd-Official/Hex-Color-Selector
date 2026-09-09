#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/GJGarageLayer.hpp>

using namespace geode::prelude;


// ============================================================
// HEX COLOR PLAYER
// ============================================================

class $modify(HexColorPlayer, PlayerObject) {
    void update(float dt) {
        PlayerObject::update(dt);

        auto mod = Mod::get();

        if (!mod->getSettingValue<bool>("enabled"))
            return;

        auto color1 =
            mod->getSettingValue<cocos2d::ccColor3B>("color1");

        auto color2 =
            mod->getSettingValue<cocos2d::ccColor3B>("color2");

        auto glow =
            mod->getSettingValue<cocos2d::ccColor3B>("glow");

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
// HEX COLOR SELECTOR WRENCH BUTTON
// ============================================================

class $modify(HexColorGarage, GJGarageLayer) {

    bool init() {
        if (!GJGarageLayer::init())
            return false;

        auto winSize =
            CCDirector::sharedDirector()->getWinSize();

        auto menu = CCMenu::create();

        if (!menu)
            return true;

        menu->setPosition({0, 0});
        menu->setID("hex-color-selector-menu");

        // Load the wrench image.
        auto wrench =
            CCSprite::create("hex-button.png"_spr);

        if (!wrench)
            return true;

        // Make the wrench more visible.
        wrench->setScale(0.28f);

        auto button = CCMenuItemSpriteExtra::create(
            wrench,
            this,
            menu_selector(HexColorGarage::onHexColorButton)
        );

        if (!button)
            return true;

        button->setID("hex-color-selector-button");

        // More visible position:
        // upper-right area of the garage.
        button->setPosition({
            winSize.width - 65.0f,
            winSize.height - 100.0f
        });

        menu->addChild(button);

        this->addChild(menu, 100);

        return true;
    }

    void onHexColorButton(CCObject*) {
        openSettingsPopup(Mod::get());
    }
};
