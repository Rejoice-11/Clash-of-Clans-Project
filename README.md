# Clash-of-Clans-Project

部落冲突

---
A project of Clash of Clans based on [Cocos2d-x 3.17.2](https://docs.cocos.com/cocos2d-x/manual).

基于 [Cocos2d-x 3.17.2](https://docs.cocos.com/cocos2d-x/manual) 开发的部落冲突游戏项目。

> ***Relevant course***
> * Programing Paradigm 2025 (2025 年同济大学程序设计范式)

## 项目开发日志

### 2025-12-1

* Python  环境配置
* Cocos2d-x 3.17.1 环境配置
* Markdown语法学习
* github使用学习

### 2025-12-2

* 项目创建
* 项目托管 GitHub [仓库](https://github.com/Rejoice-11/Clash-of-Clans-Project)

### 2025-12-16

* 改用Cocos2d-x 3.17.2
* 学习使用visual studio 进行git管理
* 第一次方案制定(Clash-of-Clans.docx文件)
* 初步确定成员分工
  
### 2025-12-19

* 修改.gitignore

### 2025-12-20

* 实现开头加载动画
* 初步实现Villagescene界面框架
* 添加Villagescene界面可以用鼠标滚轮缩放
* 初步实现商店与进攻界面

### 2025-12-21

* 将大部分商店界面的实现分离出来
* 初步实现从商店里面选取建筑并拖到Villagescene
* 新建了GridUtils.h/.cpp，用于实现世界坐标到格子坐标的转换

### 2025-12-22

* 在Villagescene中绘制40x40的格子用于后续建筑放置
* 实现滚轮缩放的时侯，网格也会一起缩放
* ResourceBulilding.h/.cpp完成
* 现在可以从Storewindow正确拖出建筑到绘制的格子中
* DefenseBuiling.h/.cpp完成
* Storewindow可以显示初始花费
* 实现在Villagescene右上角显示当前的gold,elixir数量和最大值
* 放弃缩放功能
* 添加了从商店选择的建筑在网格中逐格移动以及边界判断
* 修复了一些已知的bug

### 2025-12-23

* 现在商店选择的建筑可以实体放置了
* 将BuildingPanel和Villagescene衔接好
* 初步实现了Battlescenen的一些画面

### 2025-12-24

* 在Battlescene里面注册了鼠标事件
* 初步实现unit基类和野蛮人类并完善
* 在Battlescene中添加了箭头表示所选的兵种卡片

### 2025-12-25

* 初步实现弓箭手类，炸弹人类，巨人类
* 完善士兵单位消失逻辑
* 在Villagescene里面实现建筑的移动功能
* 实现兵种攻击速度控制逻辑，尝试添加巨人攻击的震屏效果
* 为在Battlescene里面创建建筑做准备
* 配置了json文件来确定士兵的各项数值

### 2025-12-26

* 初步实现configmannagerunit
* 初步从armymanager实现出兵控制
* 在Villagescene里面添加了背景音效，按钮点击音效
* 添加了设置按钮，实现退出游戏功能
* 初步实现建筑升级功能
* 添加初始加载音乐以及Battlescene背景音乐
  
### 2025-12-27

* 新建unitfactory负责士兵的创建并实现在Battlescene上创建
* 更换Battlescene背景图
* 定义一个二维数组存储Battlescene网格信息
* 建筑升级和移动融合
* 初步实现了在Battlescene战斗结束后的画面
* 在Battlescene里面添加一些音效
* 实现士兵移动和攻击并把巨人震屏效果实现
* 实现建筑数量动态调整
* 添加建筑升级音效

### 2025-12-28

* 建筑升级按钮调整
* 优化Storewindow界面
* 现在放置容器类建筑会增加elixir和coin的最大值
* 现在消耗elixir,coin时侯UI会同步更新
* 加入收集elixir,coin的按钮并添加收集音效
* MilitaryBuilding类的实现，并为Militaryarrange作准备
* 现在有两种Battlescene的初始布局，用随机数来随机调用
* 修改退出游戏按钮
* 实现MillitaryArrange界面并与Battlescene绑定
* 实现防御建筑攻击逻辑
* 添加在Battlescene升级和失败的音效
* 修改了一些bug
