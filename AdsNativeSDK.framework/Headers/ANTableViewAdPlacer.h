//
//  ANTableViewAdPlacer.h
//  AdsNative-iOS-SDK
//
//  Created by Arvind Bharadwaj on 24/09/15.
//  Copyright (c) 2015 AdsNative. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "ANServerAdPositions.h"
#import "ANClientAdPositions.h"
#import "ANTableViewAdPlacerDelegate.h"

@class ANAdRequestTargeting;

////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * The `ANTableViewAdPlacer` class allows you to request native ads from the AdsNative ad server and
 * place them into a `UITableView` object.
 *
 * When an instance of this class is initialized with a table view, it wraps the table view's
 * data source and delegate in order to insert ads and adjust the positions of your regular
 * content cells.
 */
@interface ANTableViewAdPlacer : NSObject

/** @name Initializing a Table View Ad Placer */

/**
 * Creates and returns an ad placer that will insert ads into a table view at positions that can
 * be configured dynamically on the AdsNative website.
 *
 * When you make an ad request, the ad placer will ask the AdsNative ad server for the positions where
 * ads should be inserted into the table view. You can configure these positioning values by
 * editing your ad unit's settings on the AdsNative website.
 *
 * Using this method is equivalent to calling
 * +placerWithTableView:viewController:adPositions:defaultAdRenderingClass: and passing in an
 * `ANServerAdPositions` object as the `positions` parameter.
 *
 * @param tableView The table view in which to insert ads.
 * @param controller The view controller which should be used to present modal content.
 * @param defaultAdRenderingClass The class that will be used to render ads. This class must
 * implement the `ANAdRendering` protocol and must be a subclass of `UITableViewCell`.
 *
 * @return An `ANTableViewAdPlacer` object.
 */
+ (instancetype)placerWithTableView:(UITableView *)tableView viewController:(UIViewController *)controller defaultAdRenderingClass:(Class)defaultAdRenderingClass;

/**
 * Creates and returns an ad placer that will insert ads into a table view at specified positions.
 *
 * When using this method, there are two options for controlling the positions where ads appear
 * within the table view.
 *
 * First, you may pass an `ANServerAdPositions` object as the `positions` parameter, which tells
 * the ad placer to obtain positioning information dynamically from the ad server, which you can
 * configure on the AdsNative website. In many cases, this is the preferred approach, since it allows
 * you to modify the positions without rebuilding your application. Note that calling the
 * convenience method +placerWithTableView:viewController:defaultAdRenderingClass: accomplishes
 * this as well.
 *
 * Alternatively, if you wish to hard-code your positions, you may pass an `ANClientAdPositions`
 * object instead.
 *
 * @param tableView The table view in which to insert ads.
 * @param controller The view controller which should be used to present modal content.
 * @param positions The positions object that specifies where ads should be shown in the stream.
 * @param defaultAdRenderingClass The class that will be used to render ads. This class must
 * implement the `ANAdRendering` protocol and must be a subclass of `UITableViewCell`.
 *
 * @return An `ANTableViewAdPlacer` object.
 */
+ (instancetype)placerWithTableView:(UITableView *)tableView viewController:(UIViewController *)controller adPositions:(ANAdPositions *)positions defaultAdRenderingClass:(Class)defaultAdRenderingClass;

/** @name Requesting Ads */

/**
 * Requests ads from the AdsNative ad server using the specified ad unit ID.
 *
 * @param adUnitID A string representing a AdsNative ad unit ID.
 */
- (void)loadAdsForAdUnitID:(NSString *)adUnitID;

/**
 * Requests ads from the AdsNative ad server using the specified ad unit ID and targeting parameters.
 *
 * @param adUnitID A string representing a AdsNative ad unit ID.
 * @param targeting An object containing targeting information, such as geolocation data.
 */
- (void)loadAdsForAdUnitID:(NSString *)adUnitID targeting:(ANAdRequestTargeting *)targeting;

/**
 * The delegate of an `ANTableViewAdPlacer` instance may choose to
 * implement the `ANTableViewAdPlacerDelegate` protocol
 * to get callbacks for impressions and clicks
 */
@property (nonatomic, weak) id<ANTableViewAdPlacerDelegate> delegate;

@end

////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * The AdsNative SDK adds interfaces to the `UITableView` class to help your application with
 * responsibilities related to `ANTableViewAdPlacer`. These APIs include methods to help notify the
 * ad placer of all modifications to the original table view, as well as to simplify your
 * application code such that it does not need to perform index path manipulations to account for
 * the presence of ads.
 *
 * Since the ad placer replaces the original data source and delegate objects of your table view,
 * the SDK also provides new methods for you to set these properties such that the ad placer remains
 * aware of the changes.
 */

@interface UITableView (ANTableViewAdPlacer)

- (void)an_setAdPlacer:(ANTableViewAdPlacer *)placer;

/** @name Obtaining the Table View Ad Placer */

/**
 * Returns the ad placer currently being used for this table view.
 *
 * @return An ad placer object or `nil` if no ad placer is being used.
 */
- (ANTableViewAdPlacer *)an_adPlacer;

/** @name Setting and Getting the Delegate and Data Source */

/**
 * Sets the table view's data source.
 *
 * If your application needs to change a table view's data source after it has instantiated an ad
 * placer using that table view, use this method rather than -[UITableView setDataSource:].
 *
 * @param dataSource The new table view data source.
 */
- (void)an_setDataSource:(id<UITableViewDataSource>)dataSource;

/**
 * Returns the original data source of the table view.
 *
 * When you instantiate an ad placer using a table view, the ad placer replaces the table view's
 * original data source object. If your application needs to access the original data source, use
 * this method instead of -[UITableView dataSource].
 *
 * @return The original table view data source.
 */
- (id<UITableViewDataSource>)an_dataSource;

/**
 * Sets the table view's delegate.
 *
 * If your application needs to change a table view's delegate after it has instantiated an ad
 * placer using that table view, use this method rather than -[UITableView setDelegate:].
 *
 * @param delegate The new table view delegate.
 */
- (void)an_setDelegate:(id<UITableViewDelegate>)delegate;

/**
 * Returns the original delegate of the table view.
 *
 * When you instantiate an ad placer using a table view, the ad placer replaces the table view's
 * original delegate object. If your application needs to access the original delegate, use this
 * method instead of -[UITableView delegate].
 *
 * @return The original table view delegate.
 */
- (id<UITableViewDelegate>)an_delegate;

/** @name Notifying the Table View Ad Placer of Content Changes */

/**
 * Begin a series of method calls that insert, delete, or select rows and sections of the table
 * view.
 */
- (void)an_beginUpdates;

/**
 * Conclude a series of method calls that insert, delete, select, or reload rows and sections of
 * the table view.
 */
- (void)an_endUpdates;

/**
 * Reloads the rows and sections of the table view.
 */
- (void)an_reloadData;

/**
 * Inserts rows in the receiver at the locations identified by an array of index paths, and informs
 * the attached ad placer of the insertions.
 *
 * @param indexPaths An array of `NSIndexPath` objects that represent rows to insert into the
 * table.
 * @param animation  A constant that either specifies the kind of animation to perform when
 * inserting the cells or requests no animation.
 */
- (void)an_insertRowsAtIndexPaths:(NSArray *)indexPaths withRowAnimation:(UITableViewRowAnimation)animation;

/**
 * Deletes rows in the receiver at the locations identified by an array of index paths, and informs
 * the attached ad placer of the deletions.
 *
 * @param indexPaths An array of `NSIndexPath` objects identifying the rows to delete.
 * @param animation  A constant that either specifies the kind of animation to perform when
 * deleting the cells or requests no animation.
 */
- (void)an_deleteRowsAtIndexPaths:(NSArray *)indexPaths withRowAnimation:(UITableViewRowAnimation)animation;

/**
 * Reloads the specified rows using the given animation effect, and informs the attached ad placer
 * that the row positions may have changed.
 *
 * @param indexPaths An array of `NSIndexPath` objects identifying the rows to reload.
 * @param animation  A constant that indicates how the reloading is to be animated.
 */
- (void)an_reloadRowsAtIndexPaths:(NSArray *)indexPaths withRowAnimation:(UITableViewRowAnimation)animation;

/**
 * Moves the row at a specified location to a destination location, taking into account ads
 * inserted by the ad placer.
 *
 * @param indexPath    An index path identifying the row to move.
 * @param newIndexPath An index path identifying the row that is the destination of the row at
 * *indexPath*.
 */
- (void)an_moveRowAtIndexPath:(NSIndexPath *)indexPath toIndexPath:(NSIndexPath *)newIndexPath;

/**
 * Inserts one or more sections in the receiver, and informs the attached ad placer of the
 * insertions.
 *
 * @param sections  An index set that specifies the sections to insert in the receiving table view.
 * If a section already exists at the specified index location, it is moved down one index
 * location.
 * @param animation A constant that indicates how the insertion is to be animated.
 */
- (void)an_insertSections:(NSIndexSet *)sections withRowAnimation:(UITableViewRowAnimation)animation;

/**
 * Deletes one or more sections in the receiver, and informs the attached ad placer of the
 * deletions.
 *
 * @param sections  An index set that specifies the sections to delete from the receiving table
 * view. If a section exists after the specified index location, it is moved up one index location.
 * @param animation A constant that either specifies the kind of animation to perform when deleting
 * the section or requests no animation.
 */
- (void)an_deleteSections:(NSIndexSet *)sections withRowAnimation:(UITableViewRowAnimation)animation;

/**
 * Reloads the specified sections, and informs the attached ad placer that sections may have
 * changed.
 *
 * @param sections  An index set identifying the sections to reload.
 * @param animation A constant that indicates how the reloading is to be animated.
 */
- (void)an_reloadSections:(NSIndexSet *)sections withRowAnimation:(UITableViewRowAnimation)animation;

/**
 * Moves a section to a new location in the table view, and informs the attached ad placer.
 *
 * @param section    The index of the section to move.
 * @param newSection The index in the table view that is the destination of the move for the
 * section. The existing section at that location slides up or down to an adjoining index position
 * to make room for it.
 */
- (void)an_moveSection:(NSInteger)section toSection:(NSInteger)newSection;

/** @name Methods Involving Index Paths */

/**
 * Returns the table view cell at the specified index path.
 *
 * @param indexPath The index path locating the row in the table view.
 *
 * @return An object representing a cell of the table or `nil` if the cell is not visible or
 * *indexPath* is out of range.
 */
- (UITableViewCell *)an_cellForRowAtIndexPath:(NSIndexPath *)indexPath;

/**
 * Returns a reusable table-view cell object for the specified reuse identifier.
 *
 * @param identifier A string identifying the cell object to be reused. This parameter must not be
 * `nil`.
 * @param indexPath  The index path specifying the location of the cell. The data source receives
 * this information when asked for the cell and should just pass it along.
 *
 * @return A `UITableViewCell` object with the associated reuse identifier.
 */
- (id)an_dequeueReusableCellWithIdentifier:(NSString *)identifier forIndexPath:(NSIndexPath *)indexPath;

/**
 * Deselects a given row identified by index path, with an option to animate the deselection.
 *
 * @param indexPath An index path identifying a row in the receiver.
 * @param animated  YES if you want to animate the deselection and NO if the change should be
 * immediate.
 */
- (void)an_deselectRowAtIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated;

/**
 * Returns an index path representing the original row and section of a given table view cell,
 * calculated before any ads were inserted.
 *
 * @param cell A cell object of the table view.
 *
 * @return An index path representing the row and section of the cell or `nil` if the index path
 * is invalid or is a reference to a cell containing an ad.
 */
- (NSIndexPath *)an_indexPathForCell:(UITableViewCell *)cell;

/**
 * Returns an index path representing the original row and section at the given point.
 *
 * @param point A point in the local coordinate system of the receiver (the table view's bounds).
 *
 * @return An index path representing the row and section associated with *point* or `nil` if the
 * point is out of bounds of any row or is associated with a cell containing an ad.
 */
- (NSIndexPath *)an_indexPathForRowAtPoint:(CGPoint)point;

/**
 * Returns the original index path for the selected row, as if no ads have been inserted.
 *
 * @return The original index path for the selected row.
 */
- (NSIndexPath *)an_indexPathForSelectedRow;

/**
 * Returns an array of index paths each representing a non-ad row enclosed by a given rectangle,
 * calculated before any ads were inserted.
 *
 * @param rect A rectangle defining an area of the table view in local coordinates.
 *
 * @return An array of `NSIndexPath` objects each representing a row and section index identifying
 * a row within *rect*. Index paths representing ads are not returned.
 */
- (NSArray <NSIndexPath *> *)an_indexPathsForRowsInRect:(CGRect)rect;

/**
 * Returns an array of the original index paths for the selected rows, as if no ads have been
 * inserted.
 *
 * @return An array of the original index paths for the selected rows.
 */
- (NSArray <NSIndexPath *> *)an_indexPathsForSelectedRows;

/**
 * Returns an array of index paths each identifying a visible non-ad row in the table view,
 * calculated before any ads were inserted.
 *
 * @return An array of `NSIndexPath` objects each representing a visible non-ad row in the table
 * view. Returns `nil` if no rows are visible.
 */
- (NSArray <NSIndexPath *> *)an_indexPathsForVisibleRows;

/**
 * Returns the drawing area for a row identified by index path.
 *
 * @param indexPath An index path object that identifies a row of your original content.
 *
 * @return A rectangle defining the area in which the table view draws the row or `CGRectZero` if
 * *indexPath* is invalid.
 */
- (CGRect)an_rectForRowAtIndexPath:(NSIndexPath *)indexPath;

/**
 * Scrolls the table view so that the selected row nearest to a specified position in the table
 * view is at that position.
 *
 * @param indexPath      An index path that identifies a row in the table view by its row index and
 * its section index.
 * @param scrollPosition A constant that identifies a relative position in the receiving table view
 * (top, middle, bottom) for row when scrolling concludes.
 * @param animated       YES if you want to animate the change in position, NO if it should be
 * immediate.
 */
- (void)an_scrollToRowAtIndexPath:(NSIndexPath *)indexPath atScrollPosition:(UITableViewScrollPosition)scrollPosition animated:(BOOL)animated;

/**
 * Selects a row in the receiver identified by index path, optionally scrolling the row to a
 * location in the receiver.
 *
 * @param indexPath      An index path identifying a row in the receiver.
 * @param animated       YES if you want to animate the selection and any change in position, NO if
 * the change should be immediate.
 * @param scrollPosition A constant that identifies a relative position in the receiving table view
 * (top, middle, bottom) for the row when scrolling concludes.
 */
- (void)an_selectRowAtIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated scrollPosition:(UITableViewScrollPosition)scrollPosition;

/**
 * Returns an array of the non-ad cells that are visible in the table view.
 *
 * @return An array containing `UITableViewCell` objects, each representing a visible, non-ad cell
 * in the receiving table view.
 */
- (NSArray *)an_visibleCells;

@end
