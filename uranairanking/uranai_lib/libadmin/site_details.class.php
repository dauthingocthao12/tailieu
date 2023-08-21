<?php
class SiteDetails {

	/**
	 * idからサイト説明を呼び出す
	 *
	 * @author Azet
	 * @param int $id_
	 * @return array (DBからの一行)
	 */
	static function getById($id_) {
		global $conn;
		$data = array();
		
		// SITE details
		$sql  = "SELECT 
			s.site_id AS site_id,
			s.site_name,
			s.link_url,
			s.etc_url,
			sd.description,
			sd.presentation,
			sd.visible,
			sd.date_update
			FROM site AS s 
			LEFT JOIN site_details AS sd ON sd.site_id = s.site_id
			WHERE s.site_id={$id_}
			AND s.is_delete = 0
			LIMIT 1";

		$rs = $conn->query($sql);
		// print_r($sql);
		// print_r($rs);

		if($rs) {
			$data = $rs->fetch_assoc();
		}

		return $data;
	}


	/**
	 * サイト説明の保存
	 *
	 * @author Azet
	 * @param int $id_
	 * @return array (DBからの一行)
	 */
	static function save($data_) {
		global $conn;

		$data = $data_;

		// REPLACEって便利！
		$query = "REPLACE INTO site_details SET 
			site_id = {$data_['id']}
			,description = '".$conn->real_escape_string($data_['description'])."'
			,presentation = '".$conn->real_escape_string($data_['presentation'])."'
			,visible = {$data_['visible']}";
		// debug($query);

		return $conn->query($query);
	}


}
?>
