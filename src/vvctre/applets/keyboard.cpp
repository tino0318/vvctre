// Copyright 2020 vvctre emulator project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include <iostream>
#include "common/logging/log.h"
#include "vvctre/applets/keyboard.h"

namespace Frontend {

void SDL2_SoftwareKeyboard::Execute(const KeyboardConfig& config) {
    SoftwareKeyboard::Execute(config);

    LOG_INFO(Applet_SWKBD, "{}. Enter the text then press Ctrl+Z on Windows or Ctrl+D on Linux:",
             config.hint_text.empty() ? "No hint" : config.hint_text);

    std::string text, line;

    const auto GetText = [&] {
        text.clear();
        while (std::getline(std::cin, line)) {
            text += line.append(1, '\n');
        }
        text.pop_back();
        std::cin.clear();
    };

    GetText();

    ValidationError error;
    while ((error = ValidateInput(text)) != ValidationError::None) {
        switch (error) {
        case ValidationError::AtSignNotAllowed: {
            LOG_ERROR(Applet_SWKBD, "@ not allowed");
            break;
        }

        case ValidationError::BackslashNotAllowed: {
            LOG_ERROR(Applet_SWKBD, "\\ not allowed");
            break;
        }

        case ValidationError::BlankInputNotAllowed: {
            LOG_ERROR(Applet_SWKBD, "Blank input not allowed");
            break;
        }

        case ValidationError::ButtonOutOfRange: {
            LOG_ERROR(Applet_SWKBD, "Button out of range");
            break;
        }

        case ValidationError::CallbackFailed: {
            LOG_ERROR(Applet_SWKBD, "Callback failed");
            break;
        }

        case ValidationError::DigitNotAllowed: {
            LOG_ERROR(Applet_SWKBD, "Digits not allowed");
            break;
        }

        case ValidationError::EmptyInputNotAllowed: {
            LOG_ERROR(Applet_SWKBD, "Empty input not allowed");
            break;
        }

        case ValidationError::FixedLengthRequired: {
            LOG_ERROR(Applet_SWKBD, "Text length is not correct (should be {} characters)",
                      config.max_text_length);
            break;
        }

        case ValidationError::MaxLengthExceeded: {
            LOG_ERROR(Applet_SWKBD, "Text is too long (should be no more than {} characters)",
                      config.max_text_length);
            break;
        }

        case ValidationError::None: {
            break;
        }

        case ValidationError::PercentNotAllowed: {
            LOG_ERROR(Applet_SWKBD, "% not allowed");
            break;
        }

        case ValidationError::ProfanityNotAllowed: {
            LOG_ERROR(Applet_SWKBD, "Profanity not allowed");
            break;
        }
        }

        GetText();
    }

    switch (config.button_config) {
    case ButtonConfig::None: {
        break;
    }

    case ButtonConfig::Single: {
        Finalize(text, 0);
        break;
    }

    case ButtonConfig::Dual: {
        const std::string cancel =
            config.has_custom_button_text ? config.button_text[0] : SWKBD_BUTTON_CANCEL;
        const std::string ok =
            config.has_custom_button_text ? config.button_text[1] : SWKBD_BUTTON_OKAY;
        LOG_INFO(Applet_SWKBD, "Enter the button ({} or {}):", cancel, ok);
        std::string button;
        while (button != cancel && button != ok) {
            std::getline(std::cin, button);
        }
        if (button == cancel) {
            Finalize(text, 0);
        } else if (button == ok) {
            Finalize(text, 1);
        }
        break;
    }

    case ButtonConfig::Triple: {
        const std::string cancel =
            config.has_custom_button_text ? config.button_text[0] : SWKBD_BUTTON_CANCEL;
        const std::string forgot =
            config.has_custom_button_text ? config.button_text[1] : SWKBD_BUTTON_FORGOT;
        const std::string ok =
            config.has_custom_button_text ? config.button_text[2] : SWKBD_BUTTON_OKAY;
        LOG_INFO(Applet_SWKBD, "Enter the button ({}, {} or {}):", cancel, forgot, ok);
        std::string button;
        while (button != cancel && button != forgot && button != ok) {
            std::getline(std::cin, button);
        }
        if (button == cancel) {
            Finalize(text, 0);
        } else if (button == forgot) {
            Finalize(text, 1);
        } else if (button == ok) {
            Finalize(text, 2);
        }
    }
    }
}

void SDL2_SoftwareKeyboard::ShowError(const std::string& error) {
    LOG_ERROR(Applet_SWKBD, "{}", error);
}

} // namespace Frontend
