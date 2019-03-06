---
layout: post
title: 大規模Web服務開發技術
subtitle: 伊藤直也 田中慎司 著 李劍 譯 / 2011
date: 2019-02-09
category: book
bibtex:
  type: book
  title: 大規模Web服務開發技術
  author: 伊藤直也 田中慎司
  editor: 李劍
  publisher: 電子工業出版社
  isbn: 978-7-121-1388-3
  year: 2011
  month: July
---

## 第1課 大規模服務和小規模服務

Scale up: 提高硬件性能

Scale out: 增加服務器數量

用廉價硬件確保可擴展性

用load balancer

## 第2課 持續增長的服務和大規模化的障礙

用LVS + keepalived 這種負載均衡器+運行狀態盡視的開源軟件

逐漸推行虛擬操作系統

## 第3課 服務開發現場

實現過程中盡量書寫測試用例 - TDD

編程語言的選擇策略是同一層僅使用一種語言

## 第5課 大規模數據處理的難點

大規模 = 無法 in-memory computation

查找bottleneck:
- `top` 或 `sar` for processes
- `ps` for CPU time
- `strace` 或 `oprofile` 進行剖測

## 第6課 可擴展性的要點

硬件價格和性能不成比例

CPU擴展可借助於load balancer

I/O擴展十分困難，可借助於大規模數據庫

## 第8課 操作系統的緩存機制

LRU cache: Linux用inode編號識別文件，以文件inode編號和offset作鍵進行caching

OS內部使用Radix tree作caching搜索
- Radix tree similar to Patricia tree

## 第10課 利用局部性的分布式

Partitioning，將一個數據庫分割到多台服務器上

相應cache可以用在別的地方，整體來看，in-memory數據量就增加了

分割成島: Google/Yahoo spider分配到一個島，人類用戶分配到另一個
- Spider會訪問很久很久以前的網頁，人類不會
- 因應寬廣度，caching很難奏效

## 第13課 MySQL的橫向擴展和Partitioning

Redundancy = 4台server
- 如果只用3台，即1 master+2 slave，其中一台slave發生故障，就只剩下 1
  master+1 slave運作。然後用新server替換，就要從其中一台複製數據，
  過程中就要將餘下的一台slave停機，即將僅餘的redundancy都失去
- 如果master只作寫入，slave只作讀取，任何情況下都要有2台運作

Partitioning: 如果加RAM可以應付就不用Partitioning

## 第17課 可變字節碼和速度的感覺

Variable byte code: 正常integer = 4 bytes，但不是每一個integer都要4
byte。如果要儲存array of integer，e.g. [5, 130]

    5 = 00000000 00000000 00000000 00000101
    130 = 0000000 00000000 00000000 10000010

VBC: 7 LSB作數字，MSB作flag，只有最後一byte會assert MSB

    5 = 1000101
    130 = 00000001 10000010
    [5, 130] = 1000101 00000001 10000010

其他方法壓縮儲存: 用數值差e.g., [5, +125]
- 差的分佈呈幾何分佈

其他方法: Gamma code, delta code, Golomb code, interpolate encoding

## 第20課 Hatena Diary的關鍵字鏈接

Aho-Corasick算法: One large string (document), a series of keywords to search from document
- Keywords form a trie
- Trie: Add failed links to other nodes of the trie, e.g. `[ABCE, BCB]`
      A -> B -> C -> E
      B -> C -> B
  Fail link: `C` on first chain to second `B` on second chain, so when after
  `C` we failed to match `E`, we allow it to match `B`
- The trie becomes a state machine. We walk the document on the state machine
  and enumerate all “accept” nodes ever traversed.

## 第22課 創建Hatena關鍵字鏈接

Goal:

1. 給定keyword set
2. 輸入文章
3. 從文章找出關鍵字並返回 offset
4. 將關鍵字部分用 `<a href=”..”> .. </a>` 替換

Solution:

1. Aho-Corasick算法，用keyword set建trie (Aho-Corasick Automata)
2. 從Trie的root作BFS，失敗則從failure link返回

## 第25課 搜索系統的架構

全文搜索: grep
- Document size m, keyword size n, complexity = O(mn)
- Knuth-Morris-Pratt algorithm: O(m+n)
  - KMP algorithm is Aho-Corasick Algorithm with only one keyword
- Boyer-Moore algorithm: O(mn) in worst case, O(n/m) in best case

後綴類型
- Trie, suffix array, suffix tree
- 將可搜索的形式將文檔全部儲到記憶體中，不需要保存文檔副本

逆向索引 Inverted index
- Relate “term” and document
- 在文檔之外另建逆向索引
- 需要保存文檔副本

## 第25課 搜索引擎的內部結構

Dictionary + Postings
- Dictionary: keyword set
- Postings: term-document relationship
  - 能即時發現包含在term中的文檔

創建dictionary
- 怎樣選擇term?
- 預定字典 或 語素分析 或 用n-gram切分
  - 用wikipedia標題作字典
  - 語素分析，以詞性分詞

搜索引擎質素評測:
- Recall: 搜索結果佔相關文檔比例, coverage, 量
- Precision: 相關文檔佔搜索結果比例, accuracy, 質

參考文獻:
- Maxime Crochemore, Christophe Hancart, Thierry Lecrog, “Algorithm on String”, Cambridge University Press, 2007.
- Manning D. Christopher, Raghavan Prabhakar, Schutze Hinrich, “Introduction to Information Retrieval”, Cambridge University Press, 2008.
- Ian H. Witten, Alistair Moffat, Timothy C. Bell, “Managing Gigabytes”, Morgan Kaufmann, 1999.
- Bruce Croft, Donald Metzler, Trevor Stroham, “Search Engines: Information Retrieval in Practice”, Addison Wesley, 2009.

## 第28課 答案範例和思路

2010年:
- Twitter每秒處理1.2M tweets
- 橫向擴展用partitioning對數據進行分割
- 用MySQL+memcached和partitioning
- Partitioning用tweet發表時間為準，而非user id

## 第29課 企業軟件vs web服務

Web服務:
1. 低成本、高效率
   - 不要追求100%可靠性
2. 重視可擴展性、responsiveness的設計
   - 只需保持99%時間good responsive，將技術重點放在可擴展性上
3. 要重視開發速度
   - 企對軟件要從半年到一年前就開始規劃，進行開發測試，web基礎設施必須能靈活應對

## 第30課 雲vs自行構建基礎設施

Hatena服務規模
- 用戶1.5M，每月unique user 19M
- 每月數billion request，不包括圖像
- Peak time traffic 850Mbps
- 實體硬件600台以上，22 racks，virtualized 1300台

系統架構:
- App:Database ratio = 1:2
- 服務器根據請求屬性分成了面向用戶和面向爬蟲
- 架構: Reverse proxy (3), Squid (2), memcached (5), App server (5 user, 6
  spider), DB (2 content, 14 entry, 2 html, 6 keyword), load balancer (2) Other
  services: Search (2), categorize (1), related doc (2), hadoop (2), worker (2)

## 第31課 層和可擴展性

每月1M page view可用一台server處理

## 第33課 保証冗餘性

應對策略: 用load balancer作failover和failback
- Failover: 自動離線
- Failback: 恢複正常的server再次上線
- Load balancer自動health check判斷是否需要failover/failback

數據庫服務器
- Master冗餘化比較困難
- Multi-master = 雙向replication，會有延遲和短時間內(millisecond)數據不一致
- MySQL multimaster: 多個服務器之間用Virtual Router Redundancy Protocol互相監視
  - active/standby架構
  - 只有一active，所有寫入均向active寫入
  - 一旦active停機，standby就提升為active，變成新的master
  - 修復後failback，成為新standby或重新成為active

Hatena 2009年8月，一天消耗幾十GB存儲量，一個月2TB

增加單台服務器的儲存量 = 保存文件數激增 = I/O性能成瓶頸

## 第34課 系統穩定化

用保証冗餘性的方法讓系統更穩定。

避免用盡CPU，以免故障時處理能力不足。實際應只使用7成左右

Slashdot效應、digg效應: 加上squid之類的緩存服務器，結未登錄的用戶返問緩存內容

Example: Nintendo DSi
- 加星星會有Super Mario吃金幣的叮鈴聲，因此孩子會加上100個、1000個星星
- Ugo memo上線之前，全部星星有幾千萬個，上線後增大到上億。原本數GB的數據庫變成幾十GB

## 第35課 系統穩定對策

為了維持適當的餘量，在極限的70%運營

自動DoS判斷: mod_dosdetector
- 一分鐘之內同一IP地址發送的請求過多，就返回403

資源過度使用時自動重啟系統
- Memory leak protection

自動終止耗時查詢
- Kill SQL running too long
- 未能修改代碼時的臨時對策

## 第36課 虛擬化技術

Hatena用Xen (CentOS 5.2, Xen 3.0.3)
- Local disk LVM partition
- monit監視平均負載，一旦超過threshold，就執行Apache重啟或操作系統重啟

Virtualization overhead:
- CPU speed: 2-3%
- Memory throughput: 1%
- Network throughput: 50%
- I/O performance: 5%

## 第37課 硬件和提高效率

32GB RAM vs 8GB RAM + SSD
- 32GB RAM可將全部數據in-memory，不會發生I/O read，只有I/O write
- 8GB RAM I/O read非常多，但因SSD，I/O wait不明顯
- SSD效能近似於in-memory而不需用服務器專用硬件
- 兩者均達到每秒處理500次SQL SELECT的性能

SSD壽命
- 查看S.M.A.R.T. 值中的 E9 (media wearout indicator) 數值，會從100減少到0
- 用smartctl命令查看，ID 233 (Hex = E9)

## 特別篇第1課 作業隊例系統TheSchwartz、Gearman

用Task queue執行那些允許延遲的處理

## 特別篇第3課 緩存系統Squid、Varnish

兩台squid聯合運行: 使用Inter-Cache Protocol (ICP, internet draft)

兩層結構:
- 上層squid接收但不保持cache，而是將request轉發結下層squid
- 用CARP (Cache Array Routing Protocol)
- 以URL為key，轉發至適當的squid server

