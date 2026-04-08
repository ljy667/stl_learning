/* INI文件（Initialization File，初始化文件）是一种轻量级的、基于纯文本的配置文件格式，
常用于Windows操作系统和各种软件中存储设置和参数。
其结构由节（Sections）、键（Keys）和值（Values）组成，
通过记事本等工具即可轻松阅读、编辑，常用于定义程序启动参数或用户首选项。 */


/* INI:语法要点

·支持行首注释 `;` 或 `#`
· 支持行尾 inline comment
· 空行在解析时应忽略
· Section以`[name]`定义,默认section使用空字符串``
· Key-Valee®形式`key=value'(两侧可有空白)
·`a=b=c:以第一个`=`划分,右侧作为整个value
· 未加引号:从`=`到行尾(或注释符前),trim后为value
· 双引号包裹:保留内部字符,不受inline注释影响
· 支持转义:例如‘\n`,`|"`,`\|` */


/* 实现建议:从简单到健壮

· 先实现最小可用解析器(happy path)
· 补充边界测试(quotes,escapes, inline comment)
· 迭代添加功能:不要一蹴而就 */


/* 整体处理流程
·1.获取 Ini 字符串
·2. 按行解析
· 3. 识别 section/key-value ； 维护当前section ,解析key/value ,处理引号与转义
· 4. 保存解析结果 

. 2. std: : optional<std: : string>
. get(section, key);

任务分工视角
·- 输入处理
· -Parser编写(底层存储的数据结构设计)
·- 接口定义
. 1. bool parse(std::string_view view)
. 2. std::optional<std::string> 
     get(section,key)
*/

