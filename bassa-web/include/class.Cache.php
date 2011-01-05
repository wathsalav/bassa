<?php

require '/home/bassa/bassa/trunk/bassa-client-devel/php/bassalib.php';

class Cache {

    var $fname;
    var $size;
    var $time;
    var $goAhead;
    var $userId;

    function addCache($url, $user_id) {
        add($url, "", "", $user_id);
    }

    /*function getCache() {
        $this->$offset = 0;
        $this->$goAhead = 1;
        while ($this->$goAhead == 1) {
            $this->$userId = 19;
            $results = getUserDownloads($userId, $offset);
            if ($results == NULL) {
                $this->goAhead = 0;
                exit(0);
            } else {
                $j = 0;
                for ($i = 0; $i < 5; $i++) {
                    $count = 0;
                    foreach ($results as $record) {
                        if ($count == 6 | $count == 8 | $count == 9 | $count == 11 | $count == 12 | $count == 13) {
                            if ($count == 6 | $count == 7) {
                                if ($record[$j] == null) {
                                    $this->$goAhead = 0;
                                    exit(0);
                                }
                            }
                            if ($count == 6) {
                                $this->$fname = substr($record[$j], 1);
                            } elseif ($count == 11) {
                                $ori_size = $record[$j];
                                $this->$size = 0;
                                if ($ori_size < 1048576) {
                                    $new = round(($ori_size / 1024), 2);
                                    $this->$size = $new . "KB";
                                } elseif ($size > 1048576) {
                                    $new = round(($ori_size / 1048576), 2);
                                    $this->$size = $new . "MB";
                                } else {
                                    $this->$size = $ori_size . "b";
                                }
                            } elseif ($count == 12 | $count == 13) {
                                $this->$time = $record[$j];
                            } else {

                            }
                        }
                        $count++;
                    }
                    $j++;
                }
                $offset = $offset + 5;
            }
        }
    }

    function searchCache() {
        $S_val = $_GET['search_item'];
        static $condition = 1;
        static $offset = 0;
        static $fName;
        while ($condition) {
            $results = search($S_val, $offset, "0");
            $offset = $offset + 5;

            if ($results == NULL) {
                echo "no such data";
                $condition = 0;
            } else {
                $j = 0;
                for ($i = 0; $i < 5; $i++) {
                    $count = 0;
                    foreach ($results as $record) {
                        if ($count == 6 & $record[$j] == null) {
                            $condition = 0;
                        } else if ($count == 6) {
                            $fName = $record[$j];
                        }
                        if ($count == 10 & $record[$j] != null) {
                            list($a, $b, $c, $d, $e) = split('/', $record[$j]);
                            echo "<td>$fName</td>";
                            echo '<td><a href=http://10.16.90.98/' . $d . '/' . $e . '><img src="../images/Get Document.png" border="0"/></a></td>';
                        }
                        $count++;
                    }
                    $j++;
                }
            }
        }
    }*/

}

$cache = new Cache();
?>
