//村庄编辑（主场景）
// VillageScene.h
class VillageScene : public Scene 
{
    std::vector<std::unique_ptr<Building>> _buildings; // 逻辑实体
    cocos2d::Vector<Sprite*> _buildingSprites;         // 渲染节点

};