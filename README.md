# vcpp-nonlinier-magnetic-optimizer
This is a design optimization tool implemented in VC++ that automatically derives optimal specifications for magnetic components through characteristic calculations of non-linear magnetic circuits and extensive parametric searches.

非線形磁気回路の特性計算と大量パラメトリック探索により、磁気部品の最適仕様を自動抽出する VC++ 実装の設計最適化ツールです。

# Overview

This repository provides a high‑performance optimization engine designed for magnetic component development.
It performs nonlinear magnetic property calculations, large‑scale parametric sweeps, and multi‑objective evaluation (e.g., weight vs. loss), enabling automated extraction of optimal design candidates.

All material properties, operating conditions, and dimensional parameters used in this repository are fully abstracted and do not contain any confidential or product‑specific information.

本リポジトリは、磁気部品設計における 非線形磁気特性の計算 と
数万規模のパラメトリック探索 を高速に実行するための最適化エンジンを提供します。

材料物性、巻線仕様、寸法パラメータ、動作条件などを入力として、
特性計算 → 制約判定 → 多目的評価（重量・損失など）を自動で行い、
最適仕様を抽出する汎用的な設計支援フレームワークです。

本リポジトリに含まれるデータはすべて 抽象化されたダミー値 であり、
機密情報や製品固有情報は一切含まれていません。

# Features
- **Nonlinear Magnetic Modeling**

   Supports B‑H characteristics

   Gap structures and winding configurations

- **Large‑Scale Parametric Sweep**

   Tens of thousands of design candidates evaluated automatically

   High‑speed computation using VC++

- **Multi‑Objective Optimization**

   Weight, loss, and other metrics

   Constraint‑based filtering and ranking

- **Modular VC++ Architecture**

   Extendable core modules

   Clean separation of I/O, solver, and evaluation logic
