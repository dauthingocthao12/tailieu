<?php

class CustomMessage {
    
    const ENTITY_NAME = "custom_message";
    const SQL_COLLATION_UTF8MB4 = "utf8mb4";
    const SQL_COLLATION_UTF8 = "utf8";

    /** @var mysqli */
    private $conn;

    /** @var array */
    private $messages = [];


    /**
     * __construct 
     * 
     * @param mysqli $conn 
     */
    function __construct($conn) {
        $this->conn = $conn;
    }

    /**
     * resetDbCharset 
     * 
     * @access public
     */
    function resetDbCharset() {
        $this->conn->set_charset(self::SQL_COLLATION_UTF8);
    }

    /**
     * loadMessages 
     * 
     * @param array $message_ids  ["LABEL_A", "LABEL_B", ...]
     * @access public
     * @return $this
     */
    function loadMessages($message_ids) {

        $this->conn->set_charset(self::SQL_COLLATION_UTF8MB4);

        $messages = [];
        if (empty($message_ids)) { return $messages; }

        $quot = function($v) { return "'" . $v . "'"; };
        $in = join(",", array_map($quot, $message_ids));

        $sql  = "SELECT message_id, content ".
            " FROM " . self::ENTITY_NAME .
            " WHERE 1".
            " AND message_id IN (" . $in . ")";

        $rs = mysqli_query($this->conn, $sql);
        if($rs) {
            while($row = mysqli_fetch_assoc($rs)){
                $messages[$row['message_id']] = $row['content'];
            }
        }
        $this->messages = $messages;

        $this->resetDbCharset();
        return $this;
    }

    /**
     * @param string $message_ids 
     * @access public
     * @return string|null
     */
    function of($message_id) {
        return isset($this->messages[$message_id]) ? $this->messages[$message_id] : null;
    }

}




