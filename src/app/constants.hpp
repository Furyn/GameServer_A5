#pragma once

#include <cstdint>

constexpr std::uint16_t AppPort = 14768;

constexpr std::uint32_t TickRate = 26; //< Pour matcher avec unity
constexpr float gameTickInterval = 1.f / 30.f;
constexpr float networkTickInterval = 1.f / 30.f;

constexpr float playerBoundaryValue = 5.f;
constexpr float playerSpeed = 10.f;

constexpr float ballBoundaryValue = 5.f;
constexpr float ballSpeed = 8.f;
