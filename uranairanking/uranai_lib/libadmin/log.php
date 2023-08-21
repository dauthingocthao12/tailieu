<?php
/**
 * ログ一覧：ログファイルを読み込んで表示する
 * @author Azet
 * @param string $log_current_='' log file name
 * @param mixed $filter_ 絞込
 */
function log_listing($log_current_='', $filter_=null) {
	$data = array(
		'status' => 'OK',
		'message' => ''
	);

	// 最後の2つログファイルを出す
	$log_find_cmd = "ls -1 ".LOG_FOLDER." | tail -n 3";
	exec($log_find_cmd, $log_files);
	$data['log_files'] = $log_files;
	//pre($log_files);

	// 表示ファイル選択
	if($log_current_) {
		$data['log_current'] = $log_current_;
	}
	else {
		$data['log_current'] = $log_files[count($log_files)-1];
	}

	// FILTER
	if($filter_) {
		// ERR or OK logs only
		$log_file = LOG_FOLDER.$data['log_current'];
		$log_file = str_replace('\\', '/', $log_file);
		$command = 'grep "'.$filter_.'" '.$log_file.' | tac';
		exec($command, $log_lines);
	}
	else {
		// ログ・ファイル情報を読み込む
		$log_file = LOG_FOLDER.$data['log_current'];
		$command = 'tail -n '.LOG_LISTING_LINES.' '.$log_file.' | tac';
		exec($command, $log_lines);
	}
	// for logging
	$data['message'] = $command;

	foreach($log_lines as $line) {
		$line = trim($line);
		//pre($line);
		$line_arr = explode(' | ', $line);

		if(preg_match('/ERR/', $line)) {
			$line_arr['class'] = 'danger';
		}

		if(preg_match('/OK/', $line)) {
			$line_arr['class'] = 'success';
		}

		$data['log_lines'][] = $line_arr;
	}

	return $data;
}
