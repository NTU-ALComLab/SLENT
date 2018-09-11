regex2blif
=============

Pure Java implementation for converting regular expression to `BLIF` format.
-------------

## Clone 專案
```
$ git clone https://johnlin0420@bitbucket.org/johnlin0420/regex2blif.git
```

## 編譯專案
此專案採用[Maven](https://maven.apache.org/)來編譯，編譯完成後成品會位於`targe`資料夾底下：
```
$ cd regex2blif
$ mvn package
```

## 用途
```
usage: 
 -b,--bits-per-char <arg>   bits per character. Default: 8
 -d,--dot <arg>             output graphviz dotty format file. (Optional)
 -l,--log-level <arg>       Log level: DEBUG, INFO, WARN, ERROR, FATAL. Default: DEBUG
 -m,--model-name <arg>      BLIF model name. Must set with -o option. Default: 'regex'
 -o,--output <arg>          output BLIF file
 -r,--regex <arg>           regular expression
```

## 使用範例
* 正規表達式只接受`apple`此一單字:
    ```
    $ java -jar target/regex2blif-0.0.1-SNAPSHOT.jar -r apple -d /tmp/test.dot -o /tmp/test.blif
    ```
    以上 `-r`參數後面接的是正規表達式，`-o`參數後面接的是輸出BLIF檔的位置。 `-d` 參數可省略，表示輸出一份該正規表達式的State-Transition Diagram 的[Graphviz](http://www.graphviz.org/)圖檔。

 > 1.詳細的正規表達式文法，請參閱[此處](http://www.brics.dk/automaton/doc/index.html)
 
 > 2.可以把生成的`*.dot` Graphviz 圖檔內容，複製貼上到[WebGraphviz](http://www.webgraphviz.com/) 網站來即時繪製圖檔


* 正規表達式只接受`iphone`或者`htc`兩個單字:
    ```
    $ java -jar target/regex2blif-0.0.1-SNAPSHOT.jar -r \(iphone\|htc\) -d /tmp/test.dot -o /tmp/test.blif
    ```
    
    **注意:** 此處原先的正規表達式為`(iphone|htc)`，但是在Command Line 當中必須使用`\`符號來跳脫Command Line特殊字元 `|`,`(`,`)`, 因此結果變成`\(iphone\|htc\)`

* 正規表達式接受**任意字**(任意字元出現任意次數):
    ```
    java -jar target/regex2blif-0.0.1-SNAPSHOT.jar -r .\* -d /tmp/test.dot -o /tmp/test.blif
    ```
    
    **注意:** 此處原先的正規表達式為`.*`，但是在Command Line 當中必須使用`\`符號來跳脫Command Line特殊字元 `*`, 因此結果變成`.\*`

* 正規表達式接受任意字元出現一次以上:
    ```
    java -jar target/regex2blif-0.0.1-SNAPSHOT.jar -r .+ -d /tmp/test.dot -o /tmp/test.blif
    ```
    
    正規表達式`+`表示字元出現一次以上
    

* 正規表達式接受任意字元出現零次或者一次:
    ```
    java -jar target/regex2blif-0.0.1-SNAPSHOT.jar -r .\? -d /tmp/test.dot -o /tmp/test.blif
    ```
    
    **注意:** `?`表示出現零次或者一次。此處原先的正規表達式為`.?`，但是在Command Line 當中必須使用`\`符號來跳脫Command Line特殊字元 `?`, 因此結果變成`.\?`

* 正規表達式接受任意字元出現3~5次:
    ```
    java -jar target/regex2blif-0.0.1-SNAPSHOT.jar -r .\{3,5\} -d /tmp/test.dot -o /tmp/test.blif
    ```
    
    **注意:** 此處原先的正規表達式為`.{3,5}`，但是在Command Line 當中必須使用`\`符號來跳脫Command Line特殊字元 `{`,`}`, 因此結果變成`.\{3,5\}`

* 正規表達式接受任意字元出現5次以上:
    ```
    java -jar target/regex2blif-0.0.1-SNAPSHOT.jar -r .\{5,\} -d /tmp/test.dot -o /tmp/test.blif
    ```
* 正規表達式接受一個大寫字母出現一次以上:
    ```
    java -jar target/regex2blif-0.0.1-SNAPSHOT.jar -r [A-Z]+ -d /tmp/test.dot -o /tmp/test.blif
    ```
    * 正規表達式 `[A-Z]`用來表達大寫字母集合。類似的用法還有`[0-9]`(數字字元集合)、`[a-z]`(小寫字母集合)、`[A-Za-z]`(大小寫字母集合)、`[A-Za-z0-9]`(大小寫與數字集合)

* 正規表達式接受『不是大小寫字母』的字元出現一次以上:
    ```
    java -jar target/regex2blif-0.0.1-SNAPSHOT.jar -r [^A-Za-z] -d /tmp/test.dot -o /tmp/test.blif
    ```
    * 集合表示法如果以`^`開頭，代表相反的集合

* 正規表達式不接受任何字(Empty Language):
    ```
    java -jar target/regex2blif-0.0.1-SNAPSHOT.jar -r \# -d /tmp/test.dot -o /tmp/test.blif
    ```
    
    **注意:** 用來表達Empty Language的正規表達式原先為`#`，但是在Command Line 當中必須使用`\`符號來跳脫Command Line特殊字元 `#`, 因此結果變成`\#`。 另外，如果要表達原始的`#`字元，則需使用兩次跳脫字元`\\\#`來表達一個井字: 前兩條`\\`代表一次`\`，最後一條`\`則是用來跳脫`#`。

## Log 層級調整
可以在Command Line 當中使用`-l`參數來調整Log資訊層級。

* 使用`FATAL`層級會使得程式只會在嚴重出錯時才會印出Log, 其餘正常運作的情況下，不會有任何Log:

```
$ java -jar target/regex2blif-0.0.1-SNAPSHOT.jar -l FATAL -r apple -d /tmp/test.dot -o /tmp/test.blif
```


     