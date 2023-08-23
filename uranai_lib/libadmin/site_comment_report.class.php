<?php

class SiteCommentReport {

//           _   _   _                 
//  ___  ___| |_| |_(_)_ __   __ _ ___ 
// / __|/ _ \ __| __| | '_ \ / _` / __|
// \__ \  __/ |_| |_| | | | | (_| \__ \
// |___/\___|\__|\__|_|_| |_|\__, |___/
//                           |___/     

	// この設定を削除・変更するのではなく、追加すること！
	// !!! 既存の設定を変えないで下さい。!!!
	// 既にできているDB情報が変わってしまうため
	static $violations = array(
		1 => 'その他', // その他は１のまま
		2 => 'アダルト及び公序良俗に反する内容が含まれている',
		3 => '個人情報が記載されている',
		4 => '誹謗中傷等の書き込み'
	);

	// 表示したい違反カテゴリを設定して下さい。
	// 下記の順番でユーザに表示されます
	static $violationsActive = array(2, 3, 4, 1);


//                 _   _               _     
//  _ __ ___   ___| |_| |__   ___   __| |___ 
// | '_ ` _ \ / _ \ __| '_ \ / _ \ / _` / __|
// | | | | | |  __/ |_| | | | (_) | (_| \__ \
// |_| |_| |_|\___|\__|_| |_|\___/ \__,_|___/
//                                           

	static function getViolationsForUser() {
		$violations = array();
		foreach(self::$violationsActive as $va) {
			$violations[$va] = self::$violations[$va];
		}

		return $violations;
	}

	/**
	 * コメントの報告を出す
	 *
	 * @author Azet
	 * @param array $comment_ids_
	 * @return array [comment_id => array[]]
	 */
	static function getNewReports($comment_ids_) {
		global $conn;
		$reports = array();

		$ids = implode(', ', $comment_ids_);
		$sql = "SELECT * FROM site_comment_report
		WHERE comment_id IN ($ids) 
		AND status=''
		ORDER BY comment_id, date_create";
		$rs = $conn->query($sql);
		// debug($sql);

		if($rs) {
			while($report = $rs->fetch_assoc()) {
				$comment_id = $report['comment_id'];
				if(!is_array($reports[$comment_id])) {
					$reports[$comment_id] = array();
				}
				$reports[$comment_id][] = $report;
			}
		}
		// debug($reports);

		return $reports;
	}


	/**
	 * コメントの報告を出す
	 *
	 * @author Azet
	 * @param int $id_ コメントのID
	 * @return array of reports
	 */
	static function getReportsForComment($id_) {
		global $conn;
		$reports = array();

		$sql = "SELECT * FROM site_comment_report
		WHERE comment_id = $id_
		ORDER BY date_create DESC";
		$rs = $conn->query($sql);

		if($rs) {
			while($row = $rs->fetch_assoc()) {
				$reports[] = $row;
			}
		}

		return $reports;
	}


	static function readReport($id_) {
		global $conn;

		$jsonAnswer = array(
			'status' => 'ERR',
			'message' => 'サーバ側にエラーになりました',
			'newStatus' => ''
		);

		$sql = "UPDATE site_comment_report
			SET status = 'read',
			date_update = NOW()
			WHERE comment_report_id = $id_
			LIMIT 1";
		$ok = $conn->query($sql);

		if($ok) {
			$jsonAnswer['status']    = 'OK';
			$jsonAnswer['message']   = '';
			$jsonAnswer['newStatus'] = 'read';
		}

		header("content-type: application/json"); // important!
		die(json_encode($jsonAnswer));
	}
}


/**
 * 報告カテゴリ番号を日本語に変換
 * converting report category number into japanese words
 */
function smarty_modifier_commentReportCategoryName($comment_cat_id_) {
	return SiteCommentReport::$violations[$comment_cat_id_];
}
