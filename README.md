# vcpp-nonlinier-magnetic-optimizer
A VC++ implementation for nonlinear magnetic circuit modeling, CSV‑driven parameter generation, and large‑scale multi‑objective optimization of magnetic components.

非線形磁気回路計算、CSV 駆動のパラメータ生成、大規模パラメトリック探索、および多目的最適化を行う VC++ 実装の磁気部品設計エンジンです。

<div style="display: flex; justify-content: space-between; align-items: center;">
  <img width="400" height="250" alt="DCDC"
       src="https://github.com/user-attachments/assets/8c43c0fd-27ec-48da-b2e7-c4ad74b3a625" />
  <img width="440" height="240" alt="tradeoff"
       src="https://github.com/user-attachments/assets/5d11899f-d6ad-4067-b04d-4ddd869752c7" />
</div>

# Overview

This repository provides a high‑performance optimization engine designed for magnetic component development.
It performs:

- Nonlinear magnetic circuit computation based on BH curves loaded from CSV

- Large‑scale parametric sweep across geometric and winding parameters

- Constraint‑based evaluation (dimensions, weight, current, loss, thermal limits)

- Multi‑objective ranking to extract optimal design candidates

All material properties, operating conditions, and dimensional parameters are fully abstracted and loaded from external CSV files.
No confidential or product‑specific information is included.

本リポジトリは、磁気部品設計に必要な以下の処理を自動化します：

- **BH カーブを CSV から読み込む非線形磁気計算**

- **6 次元パラメトリック探索（寸法・巻線・ギャップ・ターン数）**

- **寸法・重量・損失・電流・熱量などの制約評価**

- **多目的最適化による上位仕様の抽出**

材料物性・運転条件・寸法パラメータはすべて 外部 CSV から読み込む抽象化データであり、
機密情報は一切含まれていません。

# Features
- **Nonlinear Magnetic Modeling**

   BH curve import from CSV

   Nonlinear inductance calculation (Cal_L)

   Magnetic flux density & field strength evaluation (Cal_BandH)

   Gap reluctance and iron path modeling

- **CSV‑Driven Architecture**

   Material properties

   BH curve data

   Parameter ranges (gap, turns, dimensions)

   Operating conditions

   Constraint thresholds

- **Large‑Scale Parametric Sweep**

   Six‑dimensional exploration:

     core height

     core width

     wire thickness

     wire width

     turns

     gap

   Tens of thousands of candidates evaluated automatically

- **Constraint Evaluation**

   Dimensional limits

   Weight limit

   Current limit

   Loss limit (DC/AC copper loss, iron loss)

   Thermal constraints

   Minimum inductance requirements

- **Multi‑Objective Optimization**

   Weight vs. loss

   Constraint filtering

   Ranking and extraction of top candidates

- **非線形磁気モデル**

   CSV から BH カーブを読み込み

   Cal_L による非線形インダクタンス計算

   Cal_BandH による磁束密度・磁界強度計算

   ギャップ磁路・鉄磁路のモデル化

- **CSV 駆動アーキテクチャ**

   材料物性

   BH カーブ

   パラメータ範囲

   運転条件

   しきい値

- **大規模パラメトリック探索（6 重ループ）**

   コア高さ

   コア幅

   巻線厚み

   巻線幅

   ターン数

   ギャップ

- **制約評価**

   寸法制約

   重量制約

   損失制約（DC/AC 銅損・鉄損）

   電流制約

   熱量制約

   Lmin 制約

- **多目的最適化**

   重量 vs 損失

   制約フィルタリング

   上位仕様の抽出
  
# Repository Structure
 ``` 
MagneticCircuitOptimizer/
│
├── docs/
│   ├── architecture.md
│   ├── algorithm_overview.md
│   └── tradeoff_analysis.md
│
├── src/
│   ├── CORECALC5XDoc.cpp        // Optimization calc engine
│   ├── CalcArray.cpp      // Data structure
│   ├── CalcData.cpp  // Conditions
│   
│
├── examples/
│   ├── sample_materials.csv
│   ├── sample_conditions.csv
│   └── sample_parameters.csv
│
├── tests/
│   ├── unit/
│   └── integration/
│
└── README.md
 ``` 

# Build & Run
Requirements
Visual C++ (MSVC)

# Notes
This repository introduces a generalized approach to magnetic component optimization.

No confidential material properties, operating conditions, or product specifications are included.

All data is abstracted for demonstration purposes.

# License
MIT License
