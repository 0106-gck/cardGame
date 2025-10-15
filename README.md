CardGame 项目说明文档

1. 项目概述

CardGame 是一个基于 Cocos2d-x 3.17.2 开发的纸牌游戏示例项目，采用 MVC（Model-View-Controller）架构，展示了如何使用 Cocos2d-x 引擎开发简单纸牌游戏。

核心设计理念是将 数据、界面、逻辑 分离，使代码清晰、易维护、可扩展。

---

2. 核心功能

- MVC 架构
  - Model（模型层）: 管理游戏数据，如 CardModel、GameModel、UndoModel。
  - View（视图层）: 负责界面显示和动画，如 CardView、GameView。
  - Controller（控制层）: 处理用户交互和游戏逻辑，如 GameController。
- JSON 配置加载
  - 支持从 JSON 文件加载纸牌布局，如 Resources/level_1.json。
  - 易于修改关卡数据，无需改动代码。
- 牌交互操作
  - 点击手牌或桌面牌可移动牌。
  - 支持动画移动牌到目标位置。
  - 可扩展拖拽、翻牌等操作。
- 撤销操作
  - 通过 UndoManager 与 UndoModel 管理操作记录。
  - 可回退最近一次操作，包括手牌区交换和桌面牌移动。
- 可扩展性
  - 新增牌类型或游戏规则，只需扩展 Model 和 Controller。
  - View 层可增加动画和 UI 美化，不影响逻辑。

---

3. 运行要求

- Cocos2d-x: 3.17.2 版本。
- CMake: 构建项目。
- Visual Studio (Windows) 或其他支持 CMake 的 IDE。
- 支持多平台运行，但资源路径可能需调整。

---

4. 项目结构

    CardGame/
    │
    ├─ Classes/                  # 游戏源代码
    │  ├─ models/                # 数据模型
    │  │   ├─ CardModel.h/.cpp
    │  │   ├─ GameModel.h/.cpp
    │  │   └─ UndoModel.h/.cpp
    │  ├─ views/                 # 视觉组件
    │  │   ├─ CardView.h/.cpp
    │  │   └─ GameView.h/.cpp
    │  ├─ controllers/           # 游戏逻辑控制器
    │  │   └─ GameController.h/.cpp
    │  ├─ managers/              # 管理器，如 UndoManager
    │  │   └─ UndoManager.h/.cpp
    │  └─ configs/loaders/       # JSON 配置加载器
    │      └─ LevelConfigLoader.h/.cpp
    │
    ├─ Resources/               # 游戏资源
    │  ├─ images/                # 牌、背景等图像
    │  └─ level_1.json           # 关卡数据
    │
    ├─ proj.win32/              # Windows 特定 VS 项目文件
    └─ CMakeLists.txt           # CMake 构建配置

---

5. 数据流示意

    User Click --> GameController --> GameModel
                       |             |
                       v             v
                   GameView <------ CardView
    
    Undo Button --> UndoManager --> UndoModel

说明：

- 用户点击事件由 Controller 捕获并处理。
- Controller 更新 Model 数据并调用 View 刷新显示。
- View 层只负责显示，不保存逻辑状态。
- 撤销操作通过 UndoManager 操作 UndoModel 数据，并触发 View 更新。

6. 总结

此项目示例清晰地展示了 Cocos2d-x 游戏开发的 MVC 架构方法，便于后续扩展和维护。核心是 数据、界面、逻辑分离，JSON 配置提供了灵活的关卡管理，Undo 功能提供了可回退操作，整体结构清晰、可扩展性高。
# cardGame
