<?php

class Zodiac000118 extends UranaiPlugin {

    /**
     * UranaiPlugin->getParentDataを参照ください
     */
	function run($CONTENTS) {
        return $this->getParentData($CONTENTS);
	}


    /**
     * UranaiPlugin->getParentDataTopicを参照ください
     */
	function topic_run($CONTENTS) {
		return $this->getParentDataTopic($CONTENTS);
	}
}