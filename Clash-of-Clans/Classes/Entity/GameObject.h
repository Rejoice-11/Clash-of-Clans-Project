//最顶层基类（含ID、位置、状态）
#pragma once

#include "cocos2d.h"
#include <string>

/**
 * @brief 所有游戏实体的最顶层基类（建筑、单位、特效等）
 * @note 不直接实例化，仅提供通用属性与接口
 */
class GameObject 
{
public:
    virtual ~GameObject() = default;

    // 唯一标识符（用于网络/存档）
    int getId() const { return _id; }
    void setId(int id) { _id = id; }

    // 世界坐标（像素）
    const cocos2d::Vec2& getPosition() const { return _position; }
    void setPosition(const cocos2d::Vec2& pos) { _position = pos; }

    // 当前状态（建造中/正常/被摧毁）
    enum class State 
    {
        BUILDING,
        ACTIVE,
        DESTROYED
    };
    State getState() const { return _state; }
    void setState(State state) { _state = state; }

    // 类型标识（避免 dynamic_cast 滥用）
    enum class Type 
    {
        BUILDING,
        UNIT
    };
    virtual Type getType() const = 0; // 纯虚函数，强制子类实现

protected:
    explicit GameObject(int id = -1) : _id(id), _state(State::ACTIVE) {}

private:
    int _id;
    cocos2d::Vec2 _position;
    State _state;
};