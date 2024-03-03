<?php
    error_reporting(0);
    ini_set('max_execution_time', 600);

    //error log file
    const LOG_FILE = "/home/xs383367/chataibot.tech/public_html/uranai/batch/error.log";

    //api information
    const API_KEY = "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";
    const ORGANIZATION = "org-i4aEhcVsu4Pj7tp705qHlYPH";
    const API_URL = 'https://api.openai.com/v1/chat/completions';

    //db information
    const DB_HOST = "localhost";
    const DB_NAME = "xs383367_uranailinebot";
    const DB_USER = "xs383367_uranaib";
    const DB_PASSWORD = "E2bUfth3";

    const horoscope_info = "horoscope_info";

    $assign_zodiacs = 0;
    if(!empty($argv)) {
        $assign_zodiacs = $argv[1];
    }

    Log::write(0, "Start run batch. horo_id=". $assign_zodiacs);

    $zodiac_arr = [
        1 => ['name' => '牡羊座', 'date_start' => '03-21', 'date_end' => '04-19'],
        2 => ['name' => '牡牛座', 'date_start' => '04-20', 'date_end' => '05-20'],
        3 => ['name' => '双子座', 'date_start' => '05-21', 'date_end' => '06-21'],
        4 => ['name' => '蟹座', 'date_start' => '06-22', 'date_end' => '07-22'],
        5 => ['name' => '獅子座', 'date_start' => '07-23', 'date_end' => '08-22'],
        6 => ['name' => '乙女座', 'date_start' => '08-23', 'date_end' => '09-22'],
        7 => ['name' => '天秤座', 'date_start' => '09-23', 'date_end' => '10-23'],
        8 => ['name' => '蠍座', 'date_start' => '10-24', 'date_end' => '11-22'],
        9 => ['name' => '射手座', 'date_start' => '11-23', 'date_end' => '12-21'],
        10 => ['name' => '山羊座', 'date_start' => '12-22', 'date_end' => '01-19'],
        11 => ['name' => '水瓶座', 'date_start' => '01-20', 'date_end' => '02-18'],
        12 => ['name' => '魚座', 'date_start' => '02-19', 'date_end' => '03-20'],
    ];
    
    $datas = [];
    //if this handle for all the zodiacs
    if(empty($assign_zodiacs)) {
        foreach ($zodiac_arr as $zodiac) {
            //call API and get the result
            $data = handle_data($zodiac);
            if(!empty($data)) {
                $datas[] = $data;
            }
            usleep(5000);
        }
        if(count($datas) < 12) {
            Log::write(1, "Error: Total is not enough 12 zodiacs.");
        }
    } else {    //for 1 zodiac assign by user
        $data = handle_data($zodiac_arr[$assign_zodiacs]);
        if(!empty($data)) {
            $datas[] = $data;
        }
    }

    $sql = "";
    if(!empty($datas)) {
        $db = new DB();
        $select_sql = " SELECT * FROM ". horoscope_info. " WHERE get_date = '". date('Y-m-d'). "'";
        $exist_zodiacs = $db->select($select_sql, 'horo_name');

        foreach($datas as $rec) {
            if(isset($exist_zodiacs[$rec['horo_name']])) {
                $sql_update = "UPDATE ". horoscope_info. 
                    " SET horo_name = '". legal_data($rec['horo_name']). "',".
                    " date_start = '". legal_data($rec['date_start']). "',".
                    " date_end = '". legal_data($rec['date_end']). "',".
                    " get_date = '". legal_data($rec['get_date']). "',".
                    " total_fortune_msg = '". legal_data($rec['total_fortune_msg']). "',".
                    " love_fortune_msg = '". legal_data($rec['love_fortune_msg']). "',".
                    " money_fortune_msg = '". legal_data($rec['money_fortune_msg']). "',".
                    " health_fortune_msg = '". legal_data($rec['health_fortune_msg']). "',".
                    " mk_flg = 0".
                    " WHERE id = ". $exist_zodiacs[$rec['horo_name']]['id'];
                if(!$db->exec_query($sql_update)) {
                    Log::write(1, "horo={$rec['horo_name']} Error:UPDATE failed.");
                } else {
                    continue;
                }
            }

            $sql .= " (".
                "'". legal_data($rec['horo_name']). "',".
                "'". legal_data($rec['date_start']). "',".
                "'". legal_data($rec['date_end']). "',".
                "'". legal_data($rec['get_date']). "',".
                "'". legal_data($rec['total_fortune_msg']). "',".
                "'". legal_data($rec['love_fortune_msg']). "',".
                "'". legal_data($rec['money_fortune_msg']). "',".
                "'". legal_data($rec['health_fortune_msg']). "',".
                "'". $rec['ins_date']. "',".
                "0".
                "),";
        }

        if($sql) {
            $sql = trim($sql, ",");
            $sql = " REPLACE INTO ". horoscope_info.
                "(horo_name, date_start, date_end, get_date, total_fortune_msg, love_fortune_msg, money_fortune_msg, health_fortune_msg, ins_date, mk_flg) ".
                "VALUES ".
                $sql;
            $db = new DB();
            if(!$db->exec_query($sql)) {
                exit();
            }

            Log::write(0, "Finish.");
        }
    }

function handle_data($zodiac, $loop = 0) {
    try {
        $z_name = $zodiac['name'];
        $date_start = $zodiac['date_start'];
        $date_end = $zodiac['date_end'];
        $tomorrow = date('Y-m-d', strtotime('+1 day'));
        $target_date = date("n月j日",strtotime("1 day"));
        $headers = array(
            "Authorization: Bearer ". API_KEY,
            "OpenAI-Organization: ". ORGANIZATION, 
            "Content-Type: application/json"
        );

        // Define messages
        $messages = array(
            [
                "role" => "system",
                "content" =>
                    "あなたはプロの12星座から占いをおこなう女性の占い師です。
                    占い師として振る舞いなさい。
                    {$target_date}の{$z_name}の運勢を占いなさい。
                    total_fortune_msgには総合運についての占い結果を80文字以上100文字以内で出力しなさい。文の始まりは「{$target_date}の総合運は」絶対にしなさい。
                    love_fortune_msgには恋愛運についての占い結果を80文字以上100文字以内で出力しなさい。文の始まりは「{$target_date}の恋愛運は」絶対にしなさい。
                    money_fortune_msgには金運についての占い結果を80文字以上100文字以内で出力。文の始まりは「{$target_date}の金運は」絶対にしなさい。
                    health_fortune_msgには健康運についての占い結果を80文字以上100文字以内で出力。文の始まりは「{$target_date}の健康運は」絶対にしなさい。
                    その結果をjson形式で出力しなさい。
                    出力形式は以下です。
                    {
                    \"total_fortune_msg\":\"AAAA...\",
                    \"love_fortune_msg\":\"BBBB...\",
                    \"money_fortune_msg\":\"CCCC...\",
                    \"health_fortune_msg\":\"DDDD...\",
                    }"
            ]
        );
        
        // Define data
        $data = array();
        $data["model"] = "gpt-3.5-turbo";
        $data["messages"] = $messages;

        // init curl
        $curl = curl_init(API_URL);
        curl_setopt($curl, CURLOPT_POST, 1);
        curl_setopt($curl, CURLOPT_POSTFIELDS, json_encode($data));
        curl_setopt($curl, CURLOPT_HTTPHEADER, $headers);
        curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);
        
        $result = curl_exec($curl);
        if (curl_errno($curl)) {
            throw new Exception("horo=$z_name Error: chatGPT didn't respond.");
        }
        curl_close($curl);

        $result = json_decode($result, true);
        $horoscope_arr = json_decode($result['choices'][0]['message']['content'], true);

        if(empty($horoscope_arr)) {
            throw new Exception("horo=$z_name Error: JSON is empty.");
        }

        return array(
            'horo_name'                 => $zodiac['name'],
            'date_start'                => $date_start,
            'date_end'                  => $date_end,
            'get_date'                  => $tomorrow,
            'total_fortune_msg'         => $horoscope_arr['total_fortune_msg'],
            'love_fortune_msg'          => $horoscope_arr['love_fortune_msg'],
            'money_fortune_msg'         => $horoscope_arr['money_fortune_msg'],
            'health_fortune_msg'        => $horoscope_arr['health_fortune_msg'],
            'ins_date'                  => date('Y-m-d H:i:s')
        );
    } catch (Exception $e) {
        Log::write(1, $e->getMessage());
    }

    //try one more time if zodiac can't get the data
    if(!$loop) {
        usleep(50000);
        $loop += 1;
        handle_data($zodiac, $loop);
    }
    return [];
}

function legal_data($value) {
    $value = str_replace("'", "\'", $value);
    return $value;
}

class DB {
    protected $conn;

    function __construct() {
        // Create connection
        $this->conn = new mysqli(DB_HOST, DB_USER, DB_PASSWORD, DB_NAME);

        // Check connection
        if ($this->conn->connect_error) {
            Log::write(1, " Error: cannot connected to DB host");
            return null;
        }

        $this->conn->set_charset("utf8mb4");
        return $this;
    }

    function select($sql, $key) {
        $array = [];
        $result = $this->conn->query($sql);

        if ($result->num_rows > 0) {
            while($row = $result->fetch_assoc()) {
                $array[$row[$key]] = $row;
            }
        }

        return $array;
    }

    function exec_query($sql) {
        $result = true;
        if (!$this->conn->query($sql)) {
            Log::write(1, " Error: SQL could not be executed.");
            $result = false;
        }

        $this->conn->close();
        return $result;
    }
}

class Log {
    static function write($status, $res) {
        $format = "[".date('Y-m-d H:i:s')."] error=$status msg=$res\n";
        file_put_contents(LOG_FILE, $format, FILE_APPEND);
    }
}