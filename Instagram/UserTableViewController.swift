//
//  UserTableViewController.swift
//  Instagram
//
//  Created by Vikram Nag on 6/7/18.
//  Copyright © 2018 Vikram Nag Ashoka. All rights reserved.
//

import UIKit
import Parse
class UserTableViewController: UITableViewController {

    var usernames: [String] = []
    var objectIds: [String] = []
    var isFollowing = ["": false]
    
    @IBOutlet weak var Feed: UITabBarItem!
    @IBOutlet weak var PostTab: UITabBarItem!
    var refresh:UIRefreshControl = UIRefreshControl()
    
    
    @IBAction func logOutUser(_ sender: Any) {
        PFUser.logOut()
        performSegue(withIdentifier: "logOutSegue", sender: self)
        
    }
    
    @objc func updateTable(){
        let query = PFUser.query()
        query?.whereKey("username", notEqualTo: PFUser.current()?.username!)
        
        query?.findObjectsInBackground(block: { (users, error) in
            
            if error != nil{
                print(error!)
            }
            else if let users = users{
                self.isFollowing.removeAll()
                for object in users{
                    if let user = object as? PFUser{
                        if let username = user.username{
                            if let objectId = user.objectId{
                                let userArray = username.components(separatedBy: "@")
                                self.usernames.append(userArray[0])
                                self.objectIds.append(objectId)
                                
                                let userQuery = PFQuery(className: "Following")
                                userQuery.whereKey("follower", equalTo: PFUser.current()?.objectId)
                                userQuery.whereKey("following", equalTo: objectId)
                                
                                userQuery.findObjectsInBackground(block: { (objects, err) in
                                    if let objects = objects{
                                        
                                        if objects.count > 0{
                                            self.isFollowing[objectId] = true
                                        }
                                        else{
                                            self.isFollowing[objectId] = false
                                        }
                                        
                                        if self.usernames.count == self.isFollowing.count{
                                            self.tableView.reloadData()
                                            self.refresh.endRefreshing()
                                        }
                                        
                                    }
                                })
                            }
                        }
                    }
                }
                
                
            }
        })
    }
    override func viewDidLoad() {
        super.viewDidLoad()

        updateTable()
        
        refresh.attributedTitle = NSAttributedString(string:"Pull to refresh")
        refresh.addTarget(self, action: #selector(UserTableViewController.updateTable), for: UIControlEvents.valueChanged)
        
        tableView.addSubview(refresh)
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }

    // MARK: - Table view data source

    override func numberOfSections(in tableView: UITableView) -> Int {
        // #warning Incomplete implementation, return the number of sections
        return 1
    }

    override func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        // #warning Incomplete implementation, return the number of rows
        return usernames.count
    }

    
    override func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: "reuseIdentifier", for: indexPath)

        cell.textLabel?.text = usernames[indexPath.row]
        
        if let followBoolean = isFollowing[objectIds[indexPath.row]]{
            if followBoolean == true{
                cell.accessoryType = UITableViewCellAccessoryType.checkmark
            }
            
        }

        return cell
    }
    
    override func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        let cell = tableView.cellForRow(at: indexPath)
        
        if let followBoolean = isFollowing[objectIds[indexPath.row]]{
            if followBoolean{
                isFollowing[objectIds[indexPath.row]] = false
                cell?.accessoryType = UITableViewCellAccessoryType.none
                
                let userQuery = PFQuery(className: "Following")
                userQuery.whereKey("follower", equalTo: PFUser.current()?.objectId)
                userQuery.whereKey("following", equalTo: objectIds[indexPath.row])
                
                userQuery.findObjectsInBackground(block: { (objects, err) in
                    if let objects = objects{
                        
                        for object in objects{
                            object.deleteInBackground()
                        }
                    }
                })
            }
            else{
                isFollowing[objectIds[indexPath.row]] = true
                cell?.accessoryType = UITableViewCellAccessoryType.checkmark
                
                let following = PFObject(className: "Following")
                following["follower"] = PFUser.current()?.objectId
                following["following"] = objectIds[indexPath.row]
                
                following.saveInBackground()
            }
            
        }
        
        
        
    }
 

    /*
    // Override to support conditional editing of the table view.
    override func tableView(_ tableView: UITableView, canEditRowAt indexPath: IndexPath) -> Bool {
        // Return false if you do not want the specified item to be editable.
        return true
    }
    */

    /*
    // Override to support editing the table view.
    override func tableView(_ tableView: UITableView, commit editingStyle: UITableViewCellEditingStyle, forRowAt indexPath: IndexPath) {
        if editingStyle == .delete {
            // Delete the row from the data source
            tableView.deleteRows(at: [indexPath], with: .fade)
        } else if editingStyle == .insert {
            // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view
        }    
    }
    */

    /*
    // Override to support rearranging the table view.
    override func tableView(_ tableView: UITableView, moveRowAt fromIndexPath: IndexPath, to: IndexPath) {

    }
    */

    /*
    // Override to support conditional rearranging of the table view.
    override func tableView(_ tableView: UITableView, canMoveRowAt indexPath: IndexPath) -> Bool {
        // Return false if you do not want the item to be re-orderable.
        return true
    }
    */

    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        // Get the new view controller using segue.destinationViewController.
        // Pass the selected object to the new view controller.
    }
    */

}
