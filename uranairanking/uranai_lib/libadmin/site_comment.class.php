<?php

class SiteComment {

	static $STATUS_ALL       = 'all';       // ステータス関係なく
	static $STATUS_PENDING   = 'pending';   // in review
	static $STATUS_PUBLISHED = 'published'; // OK
	static $STATUS_REJECTED  = 'rejected';  // NG
	static $STATUS_HIDDEN    = 'hidden';    // 非表示・未公開

	// 下記のデータはJSの文字列になるので、改行文字は「\\n」に書いて下さい
	static $ADMIN_TEMPLATES = array(
		"アダルト系" => "コメントの内容に、アダルト及び公序良俗に反する内容が含まれていると判断いたしました。",
		"個人情報系" => "コメントの内容に、個人を特定できる情報が含まれていると判断いたしました。",
		"誹謗中傷系" => "コメントの内容に、誹謗中傷に該当する内容が含まれていると判断いたしました。"
	);

	/**
	 * ユーザのサイトのコメントを読み込む
	 *
	 * @author Azet
	 * @param int $user_id_
	 * @param int $site_id_
	 * @return array(evaluation, comment, ...)
	 */
	static function getUserComment($user_id_, $site_id_, $status_ = null) {
		global $conn;
		$comment = null;

		// default status
		if($status_ === null) {
			// published status listing
			$status = " AND status = '{self::$STATUS_PUBLISHED}'";
		}
		else if($status_ != self::$STATUS_ALL) {
			// specific status
			$status = " AND status = '{$status_}'";
		}
		else {
			// any status
			$status = "";
		}

		// query
		$sql = "SELECT *
			FROM site_comment
			WHERE user_id=$user_id_
			AND site_id=$site_id_
			$status
			AND is_delete = 0
			ORDER BY date_create DESC
			LIMIT 1";
		$rs = $conn->query($sql);
		// debug( $sql );

		if($rs) {
			$comment = $rs->fetch_assoc();
		}

		return $comment;
	}


	/**
	 * ユーザのサイトのコメントを読み込む (管理画面用)
	 *
	 * @author Azet
	 * @param int $coment_id_ コメントのID
	 * @return array( comment data )
	 */
	static function getSingleComment($comment_id_, $withRevision_ = true) {
		global $conn;
		$comment = null;

		// query
		$sql = "SELECT 
			sc.*,
			s.site_name,
			u.handlename,
			u.email as user_email,
			u.notificationCommentPublished as user_notification_comment_published,
			u.notificationCommentRejected as user_notification_comment_rejected,
			IF(COUNT(cr.comment_report_id)>0, 1, 0) as is_reported
			FROM site_comment sc
			INNER JOIN site s ON s.site_id = sc.site_id AND s.is_delete = 0
			INNER JOIN users u ON u.user_id = sc.user_id AND u.is_delete = 0
			LEFT JOIN site_comment_report cr ON cr.comment_id = sc.site_comment_id
			WHERE site_comment_id=$comment_id_
			AND sc.is_delete = 0
			LIMIT 1";
		// debug( $sql );
		$rs = $conn->query($sql);

		if($rs) {
			$comment = $rs->fetch_assoc();
		}

		// load revision if any
		if(isset($comment['parent_revision'])) {
			$comment['revision'] = self::getSingleComment($comment['parent_revision'], false); // no recursive loop
		}

		return $comment;
	}


	/**
	 * ユーザのコメント一覧
	 * @param int $user_id_
	 * @return array() (site_details : fetch_assoc arrays)
	 */
	static function getUserComments($user_id_) {
		global $conn;
		$comments = array();

		$sql = "SELECT
			sd.*,
			COUNT(fav.favorite_id) AS likes_count,
			s.site_name,
			s.link_url,
			s.etc_url
			FROM site_comment sd
			INNER JOIN (
				SELECT MAX(date_create) as date_max
				FROM site_comment
				WHERE user_id = $user_id_ AND is_delete = 0
				GROUP BY site_id
			) lst ON lst.date_max = sd.date_create
			INNER JOIN site s ON s.site_id = sd.site_id AND s.is_delete = 0
			LEFT JOIN site_comment_favorite fav ON fav.comment_id = sd.site_comment_id AND fav.is_delete = 0
			WHERE sd.user_id = $user_id_
			AND sd.is_delete = 0
			GROUP BY sd.site_comment_id
			ORDER BY sd.date_create DESC";
		// debug($sql);

		$rs = $conn->query($sql);
		if($rs) {
			while($comment = $rs->fetch_assoc()) {
				$comments[] = $comment;
			}
		}

		return $comments;
	}


	/**
	 * サイトのコメントのページを計算する
	 * 
	 * @param array $comment_ (site_id, date_create)
	 * @return int (コメントのページ番号、規定は 1 です)
	 */
	static function findUserCommentPage($comment_) {
		global $conn;

		$status = self::$STATUS_PUBLISHED;
		$paging = Paginator::getPagingFor('site-comments');
		$comment_page = 1;

		// ref: SITECOMMENTQUERY : このクエリーを直すときに、ほかのrefもあわせてください
		$sql = "SELECT FLOOR(COUNT(*) / $paging)+1 as page_number
			FROM site_comment sc
			INNER JOIN users u ON u.user_id = sc.user_id AND u.is_delete = 0
			WHERE sc.site_id = {$comment_['site_id']}
			AND sc.status = '$status'
			AND sc.is_delete = 0
			AND sc.date_create > '{$comment_['date_create']}'
			ORDER BY sc.date_create DESC";
		// debug($sql);

		$rs = $conn->query($sql);


		if($rs) {
			$data = $rs->fetch_row();
			$comment_page = $data[0];
		}

		return $comment_page;
	}


	/**
	 * サイトのコメントを引き出す (サイト詳細ページ)
	 *
	 * @author Azet
	 * @param int $site_id_
	 * @return array (of comments)
	 */
	static function getSiteComments($site_id_, $user_id_ = 0) {
		global $conn;

		$status = self::$STATUS_PUBLISHED;
		$comments = array();

		$page = Paginator::getCurrentPageFor('site-comments'); // page set in controller
		list($start, $size) = Paginator::getLimitFor('site-comments', $page);

		// ref: SITECOMMENTQUERY : このクエリーを直すときに、ほかのrefもあわせてください
		$sql = "SELECT SQL_CALC_FOUND_ROWS
			sc.*,
			u.handlename,
			u.avatar as user_avatar,
			COUNT(fav.favorite_id) AS likes_count,
			IF(fav2.user_id, 1, 0) AS user_likes
			FROM site_comment sc
			INNER JOIN users u ON u.user_id = sc.user_id AND u.is_delete = 0
			LEFT JOIN site_comment_favorite fav ON fav.comment_id = sc.site_comment_id AND fav.is_delete = 0
			LEFT JOIN site_comment_favorite fav2 ON fav2.comment_id = sc.site_comment_id AND fav2.user_id = $user_id_ AND fav2.is_delete = 0
			WHERE sc.site_id = $site_id_
			AND sc.status = '$status'
			AND sc.is_delete = 0
			GROUP BY sc.site_comment_id
			ORDER BY sc.date_create DESC
			LIMIT $start, $size
";
		// debug($sql);

		$rs = $conn->query($sql);
		if($rs) {
			while($data = $rs->fetch_assoc()) {
				$comments[] = $data;
			}
		}

		$rs->free();
		Paginator::setRecordsCountFor('site-comments');

		return $comments;
	}


	static function getSiteEvaluationData($site_id_) {
		global $conn;
		$status = self::$STATUS_PUBLISHED;
		$data = array();

		$sql = "SELECT
			sc.evaluation
			FROM site_comment sc
			INNER JOIN users u ON u.user_id = sc.user_id AND u.is_delete = 0
			WHERE sc.site_id = $site_id_
			AND sc.status = '$status'
			AND sc.is_delete = 0
		";
		// debug($sql);

		$rs = $conn->query($sql);
		if($rs) {
			while($row = $rs->fetch_assoc()) {
				$data[] = $row;
			}
		}
		// debug($data);

		return $data;
	}


	/**
	 * 星座詳細ページのサイト一覧の為に評価データ計算するメソッド
	 * @return array [<site_id> = [evaluation_average: int, comments_count: int]]
	 */
	static function compileSitesEvaluationData() {
		global $conn;
		$status = self::$STATUS_PUBLISHED;
		$data = array();

		$sql = "SELECT
				sc.site_id,
				avg(sc.evaluation) as evaluation_average,
				count(sc.site_id) as comments_count
			FROM site_comment sc
			INNER JOIN users u ON u.user_id = sc.user_id AND u.is_delete = 0
			WHERE sc.status = '$status'
			AND sc.is_delete = 0
			GROUP BY sc.site_id
			ORDER BY sc.site_id
		";
		// debug($sql);

		$rs = $conn->query($sql);
		if($rs) {
			while($row = $rs->fetch_assoc()) {
				$id = $row['site_id'];
				$data[$id] = array(
					'evaluation_average' => $row['evaluation_average'],
					'comments_count' => $row['comments_count']
				);
			}
		}
		// debug($data);

		return $data;
	}


	/**
	 * すべてのコメントを引き出す （管理画面用）
	 *
	 * @param string $status
	 * @param int $user_id_ (filter by user)
	 * @param int $site_id_ (filter by site)
	 * @return array(comments)
	 */
	static function getSitesComments($status_, $user_id_ = 0, $site_id_ = 0) {
		global $conn;
		$comments = array();

		// 絞り込む
		if($status_ == 'reported') {
			$status_filter = " AND sc.site_comment_id IN (
				SELECT DISTINCT(comment_id) 
				FROM site_comment_report
				WHERE status = '')";
		}
		else if($status_ != self::$STATUS_ALL) {
			$status_filter = "AND status = '{$status_}'";
		}
		else {
			$status_filter = '';
		}

		$user_filter = '';
		if($user_id_ > 0) {
			$user_filter = "AND sc.user_id = $user_id_";
		}

		$site_filter = '';
		if($site_id_ > 0) {
			$site_filter = "AND sc.site_id = $site_id_";
		}

		// get page from user request
		$page = Paginator::getCurrentPageFor('admin-comments');
		list($start, $size) = Paginator::getLimitFor('admin-comments', $page);


		$sql = "SELECT SQL_CALC_FOUND_ROWS 
			sc.*,
			u.handlename,
			s.site_name
			FROM site_comment sc
			INNER JOIN (
				SELECT site_id, user_id, MAX(date_create) as date_max
				FROM site_comment
				WHERE is_delete = 0
				GROUP BY site_id, user_id
			) lst ON sc.site_id = lst.site_id AND sc.user_id = lst.user_id AND sc.date_create = lst.date_max 
			INNER JOIN site s ON s.site_id = sc.site_id AND s.is_delete = 0
			INNER JOIN users u ON u.user_id = sc.user_id AND u.is_delete = 0
			WHERE 1 $status_filter $user_filter $site_filter
			AND sc.is_delete = 0
			ORDER BY sc.date_create DESC
			LIMIT $start, $size";
		// debug($sql);

		$rs = $conn->query($sql);
		if($rs) {
			while($row = $rs->fetch_assoc()) {
				$comments[] = $row;
			}
		}

		// paginator total >>>
		$rs->free(); // important to count rows
		Paginator::setRecordsCountFor('admin-comments');
		// <<<


		return $comments;
	}


	/**
	 * ユーザのサイトのコメントを保存する
	 *
	 * @author Azet
	 * @param int site_id
	 * @param int user_id
	 * @param array $data_[
	 *  - evaluation (int: 1~5)
	 *  - comment (text)
	 * ]
	 * @return string:
	 *  - published : DBに保存OK，公開中
	 *  - pending : DBに保存ok, 管理者がリビューする
	 *  - error : DBに保存席なかった
	 */
	static function saveUserComment($site_id_, $user_id_, $data_) {
		global $conn;

		// escaping and default values
		$comment = $conn->real_escape_string(htmlentities($data_['comment'], ENT_COMPAT | ENT_HTML401, "UTF-8"));
		// コメント内容がない場合は、statusはpublishedにする（審査はいらない）
		$status = (strlen($comment)==0) ? self::$STATUS_PUBLISHED : self::$STATUS_PENDING;

		if($data_['current_status'] == self::$STATUS_REJECTED || 
			($data_['current_status'] == self::$STATUS_PUBLISHED && $status == self::$STATUS_PUBLISHED)) {
			// コメントデータを更新 (edit same revision)
			// コメントの本文無い時にも更新＋自動的にpublishedになります
			$sql_start = "UPDATE site_comment SET ";
			$sql_end = " WHERE site_comment_id = {$data_['current_id']}";
		}
		else {
			// コメントデータを追加 (new revision)
			$sql_start = "INSERT INTO site_comment SET
				parent_revision = {$data_['parent_revision']}, 
				user_id = {$user_id_}, ";
			$sql_end = "";
		}

		$sql = $sql_start . "site_id = {$site_id_},
			evaluation = {$data_['evaluation']},
			comment = '{$comment}',
			status = '{$status}'" . $sql_end;
		// debug($sql);
		// return false; // testing
		$ok = $conn->query($sql);

		if($ok && $status==self::$STATUS_PUBLISHED && $data_['current_parent']) {
			// 無効コメントを自動公開になる時に、前のレビジョン（あれば）を削除します
			self::commentDelete($data_['current_parent']);
		}

		if($ok && $status!=self::$STATUS_PUBLISHED) {
			// 管理者にコメント審査お知らせメールを送ります

			// サイト名を探す
			$site_rs = $conn->query("SELECT site_name, date_create FROM site WHERE site_id=$site_id_ LIMIT 1");
			$site_data = $site_rs->fetch_row();

			$data = array(
				'site_id'         => $site_id_,
				'site_name'       => $site_data[0],
				'date_create'     => $site_data[1],
				'user_id'         => $user_id_,
				'evaluation'      => $data_['evaluation'],
				'comment'         => $data_['comment']
			);

			// add current date
			self::sendSiteCommentMailNotification($data);
		}
		else if(!$ok) {
			// error while saving?
			$status = 'error';
		}

		return $status;
	}


	/**
	 * コメントの複数レビションを読み込む
	 *
	 * @author Azet
	 * @param array(int) $revisions_ comments ids
	 * @return array(parent_id: comment)
	 */
	static function loadRevisions($revisions_) {
		global $conn;
		$data = array();

		$revisions = implode($revisions_, ',');
		$sql = "SELECT
			sd.*,
			s.site_name,
			COUNT(fav.favorite_id) AS likes_count
			FROM site_comment sd
			INNER JOIN site s ON s.site_id = sd.site_id AND s.is_delete = 0
			LEFT JOIN site_comment_favorite fav ON fav.comment_id = sd.site_comment_id AND fav.is_delete = 0
			WHERE sd.site_comment_id IN ($revisions)
			AND sd.is_delete = 0
			GROUP BY sd.site_id";
		// debug($sql);
		$rs = $conn->query($sql);

		if($rs) {
			while($line = $rs->fetch_assoc()) {
				$line_id = $line['site_comment_id'];
				$data[$line_id] = $line;
			}
		}

		return $data;
	}


	/**
	 * コメントに対して管理者のメール情報
	 * 
	 * @param  array(comment_id)
	 * @return array(comment_id => array(admin_data))
	 */
	static function loadRejectedMailContent($comments_ids_) {
		global $conn;
		$data = array();

		$comments_ids = implode($comments_ids_, ',');
		$sql = "SELECT sca.*
			FROM site_comment_admin sca
			WHERE sca.comment_id IN ($comments_ids)
			ORDER BY date_create DESC";
		// debug($sql);
		$rs = $conn->query($sql);

		if($rs) {
			while($line = $rs->fetch_assoc()) {
				if(!$line['mail_sent']) continue;

				$line_id = $line['comment_id'];

				if(!isset($data[$line_id])) {
					$data[$line_id] = array();
				}

				$data[$line_id][] = $line;
			}
		}

		return $data;
	}


	/**
	 * ユーザのサイトコメントを非表示
	 *
	 * @author Azet
	 * @param int $user_id_
	 * @param int $comment_id_
	 * @return bool
	 */
	static function hideUserComment($user_id_, $comment_id_) {
		global $conn;

		$new_status = self::$STATUS_HIDDEN;

		$sql = "UPDATE site_comment
			SET
			status = '$new_status',
			date_update = NOW()
			WHERE user_id = $user_id_ AND site_comment_id = $comment_id_
			LIMIT 1
";
		// debug($sql);

		$ok = $conn->query($sql);

		return $ok;
	}


	/**
	 * ユーザのサイトコメントを非表示
	 *
	 * @author Azet
	 * @param int $user_id_
	 * @param int $comment_id_
	 * @return bool
	 */
	static function showUserComment($user_id_, $comment_id_) {
		global $conn;

		$new_status = self::$STATUS_PUBLISHED;

		$sql = "UPDATE site_comment
			SET status = '$new_status',
			date_update = NOW()
			WHERE user_id = $user_id_ AND site_comment_id = $comment_id_
			LIMIT 1
";

		$ok = $conn->query($sql);

		return $ok;
	}


	/**
	 * ユーザがコメントをいいね追加・外す
	 *
	 * @author Azet
	 * @param int $user_id_
	 * @param int $comment_id_
	 * @return array (commentLikes, userLikes)
	 */
	static function likeComment($user_id_, $comment_id_) {
		global $conn;
		//only for debugging
		$debug = array();

		$sql = "SELECT * 
			FROM site_comment_favorite fav
			WHERE comment_id = $comment_id_
			AND  user_id = $user_id_
			AND is_delete = 0
			LIMIT 1";
		$debug[] = $sql;

		$rs = $conn->query($sql);

		if($rs && $rs->num_rows == 0) {
			// like the comment
			$likes = true;
			$debug[] = "like the comment";

			$like_ok = $conn->query("INSERT INTO site_comment_favorite
				SET comment_id = $comment_id_, user_id = $user_id_");
		}
		else {
			// unlike the comment
			$likes = false;
			$debug[] = "unlike the comment";

			// delete previous like
			$fav_data = $rs->fetch_assoc();
			$diskike_ok = $conn->query("UPDATE site_comment_favorite
				SET is_delete = 1, date_delete = NOW()
				WHERE favorite_id = {$fav_data['favorite_id']}
				LIMIT 1");
		}

		// new stats
		$sql = "SELECT count(*) as counter
			FROM site_comment_favorite
			WHERE comment_id = $comment_id_
			AND is_delete = 0";
		$debug[] = $sql;

		$rs = $conn->query($sql);
		if($rs && $rs->num_rows == 1) {
			$comment_likes_data = $rs->fetch_row();
			$likes_counter = $comment_likes_data[0];
		}
		else {
			$likes_counter = 0;
		}

		$newData = array(
			'debug' => $debug,
			'likes_count' => $likes_counter,
			'likes' => $likes
		);

		return $newData;
	}


	/**
	 * ユーザがコメントをする時に、管理者にメールを送ります。
	 * ユーザ　→　管理者
	 * @param array $comment_ (data from DB)
	 * @return bool (success or not)
	 */
	static function sendSiteCommentMailNotification($comment_) {
		global $smarty;
		// data
		$smarty->assign('comment', $comment_);
		// template parsing
		$body = $smarty->fetch("site-comment-notification-mail.tpl");
		// email
		$mail = new mail();
		$mail->set_encoding("utf-8");
		$ok = $mail->send(
			MAIL_SENDER_EMAIL,
			MAIL_SENDER_NAME,
			COMMENT_NOTIFICATION_ADMIN_EMAIL,
			COMMENT_NOTIFICATION_EMAIL_SUBJECT,
			$body);

		return $ok;
	}


	/**
	 * ユーザが無効サれたコメントがあるか確認
	 * 
	 * @param  int
	 * @return boolean
	 */
	static function getRejectedUserComments($user_id_) {
		global $conn;

		$status = self::$STATUS_REJECTED;
		$sql = "SELECT site_comment_id
			FROM site_comment sc
			WHERE sc.status = '$status'
			AND sc.user_id = $user_id_
			AND sc.is_delete = 0";
		$rs = $conn->query($sql);

		return $rs && $rs->num_rows>0;
	}


	/**
	 * 管理者はコメント情報を保存する
	 *
	 * @author Azet
	 * @param array $data_
	 * @return bool (success or not)
	 */
	static function adminSave($data_) {
		global $conn, $smarty;

		// ====================================
		// 基本データ保存
		// ====================================
		$data = array(
			'admin_memo' => $conn->real_escape_string($data_['memo']),
			'mail_sent' => ($data_['mail_sent']=='yes' ? 1 : 0),
			'mail_content' => $conn->real_escape_string($data_['mail_content']),
			'comment_content' => $conn->real_escape_string(htmlentities($data_['comment_content'], ENT_COMPAT | ENT_HTML401, "UTF-8"))
		);

		$sql = "UPDATE site_comment
			SET admin_memo = '{$data['admin_memo']}',
			status = '{$data_['status']}',
			admin_date = NOW()
			WHERE site_comment_id = {$data_['id']}
			LIMIT 1";

		$base_rs = $conn->query($sql);

		// fetch updated comment data
		$comment = self::getSingleComment($data_['id']);

		// ====================================
		// 管理履歴を保存
		// ====================================
		$mail_sent = false;
		$action = "状況を「".smarty_modifier_commentStatusJapanese($data_['status'])."」にしました";

		if($data_['mail_sent'] == 'yes' && $data['mail_content']) {
			if($data_['status']==self::$STATUS_PUBLISHED && $comment['user_notification_comment_published']==0) {
				$action .= "\nメールを送りませんでした(ユーザの設定で)";
			}
			elseif(($data_['status']==self::$STATUS_REJECTED || $data_['status']==self::$STATUS_PENDING)
				&& $comment['user_notification_comment_rejected']==0) {
				// コメントを「公開中」から「審査中」・「無効」に変更する時に、同じメール受信設定を使う
				$action .= "\nメールを送りませんでした(ユーザの設定で)";
			}
			else {
				$action .= "\nメールを送りました";
				$mail_sent = true;
			}
		}

		// コメントを公開する
		// =================
		if($data_['status'] == self::$STATUS_PUBLISHED) {
			// 更新されたコメントの場合は、前のレビションを削除する
			if($comment['parent_revision']>0) {
				self::commentDelete($comment['parent_revision']);
			}

			// 「通信する」ボタンをyesの場合はユーザに公開お知らせメールを送ります
			if($data_['mail_sent']=='yes') {
				if($comment['user_notification_comment_published']==1) {
					$action .= "\n公開自動メールを送りました";

					// ユーザにメールを公開お知らせを送ります
					self::commentPublishedNotificationToUser($comment);
				}
				else {
					$action .= "\n公開自動メールを送りませんでした（ユーザの設定で）";
				}
			}
		}

		// DBに履歴を追加
		// ==============
		$sql_admin = "INSERT INTO site_comment_admin
			SET 
			comment_id = {$data_['id']},
			action = '$action',
			comment_content = '{$data['comment_content']}',
			mail_sent = {$data['mail_sent']},
			mail_content = '{$data['mail_content']}'";

		$admin_rs = $conn->query($sql_admin);

		// ====================================
		// メール通信（あれば）
		// ====================================
		$mail_ok = true;

		if($mail_sent) {
			// data
			$smarty->assign('comment', $comment);
			$smarty->assign('message', $data_['mail_content']);

			// template parsing
			$body = $smarty->fetch("admin-comment-notification-mail.tpl");
			// email
			$mail = new mail();
			$mail->set_encoding("utf-8");
			$mail_ok = $mail->send(
				COMMENT_NOTIFICATION_ADMIN_EMAIL,
				MAIL_SENDER_NAME,
				$comment['user_email'],
				COMMENT_ADMIN_EMAIL_SUBJECT,
				$body);
		}
		// END

		return $base_rs && $admin_rs && $mail_ok;
	}


	/**
	 * コメントを削除
	 * 使い方：管理画面からいコメントの更新を公開する時に、前のコメントを削除する
	 * 又は、ユーザが無効コメントを更新する時に、コメント本文がなければ、自動公開するため、前のコメントを削除
	 * 
	 * @param  int $comment_id_ (comment to delete)
	 * @return boolean (success or not)
	 */
	static function commentDelete($comment_id_) {
		global $conn;
		// debug("delete previous comment");

		$sql = "UPDATE site_comment SET is_delete = 1, date_delete=NOW() WHERE site_comment_id=$comment_id_ LIMIT 1";

		return $conn->query($sql);
	}


	/**
	 * コメントの管理履歴を引き出す
	 *
	 * @author Azet
	 * @param int $comment_id_ コメントのID
	 * @return array (複数履歴のデータ)
	 */
	static function getAdminHistory($comment_id_) {
		global $conn;
		$history = array();

		$sql = "SELECT * 
			FROM site_comment_admin
			WHERE comment_id = $comment_id_
			ORDER BY date_create DESC";

		$rs = $conn->query($sql);
		if($rs) {
			while($line = $rs->fetch_assoc()){
				$history[] = $line;
			}
		}

		return $history;
	}

	/**
	 * @param array $comment_ (data from getSingleComment)
	 * @return bool (success or not)
	 */
	static function commentPublishedNotificationToUser($comment_) {
		global $smarty;

		// data
		$smarty->assign('comment', $comment_);

		// template parsing
		$body = $smarty->fetch("admin-comment-published-notification-mail.tpl");
		// email
		$mail = new mail();
		$mail->set_encoding("utf-8");
		$mail_ok = $mail->send(
			COMMENT_NOTIFICATION_ADMIN_EMAIL,
			MAIL_SENDER_NAME,
			$comment_['user_email'],
			COMMENT_ADMIN_EMAIL_SUBJECT,
			$body);

		return $mail_ok;
	}


	/**
	 * サイトの一覧 (コメントのあるサイトのみ)
	 *
	 * @author Azet
	 * @return array
	 */
	static function listSites() {
		global $conn;
		$sites = array();

		$sql = "SELECT
			s.site_id,
			s.site_name
			FROM site_comment sc
			INNER JOIN site s ON s.site_id = sc.site_id AND s.is_delete = 0
			WHERE sc.is_delete = 0
			GROUP BY s.site_id, s.site_name
			ORDER BY s.site_name ASC";
		// debug($sql);

		$rs = $conn->query($sql);

		if($rs) {

			while($site = $rs->fetch_assoc()) {
				$id = $site['site_id'];
				$sites[$id] = $site['site_name'];
			}
		}

		return $sites;
	}


	/**
	 * ユーザの一覧(コメントを書いたユーザのみ)
	 *
	 * @author Azet
	 * @return array
	 */
	static function listUsers() {
		global $conn;
		$users = array();

		$sql = "SELECT
			u.user_id,
			u.handlename,
			u.email
			FROM site_comment sc
			INNER JOIN users u ON u.user_id = sc.user_id AND u.is_delete = 0
			WHERE sc.is_delete = 0
			GROUP BY u.user_id
			ORDER BY u.email, u.handlename ASC";
		// debug($sql);

		$rs = $conn->query($sql);

		if($rs) {
			while($user = $rs->fetch_assoc()) {
				$id = $user['user_id'];
				$users[$id] = $user['handlename']."&lt;".$user['email']."&gt;";
			}
			// debug($users);
		}

		return $users;
	}
}
