﻿/*
  ==============================================================================

    DocTreeViewItem.h
    Created: 16 Sep 2016 5:20:49am
    Author:  SwingCoder

  ==============================================================================
*/

#ifndef DOCTREEVIEWITEM_H_INCLUDED
#define DOCTREEVIEWITEM_H_INCLUDED

/** Repsent a doc, a dir or the project which showed in treeView. */
class DocTreeViewItem : public TreeViewItem,
                        private ValueTree::Listener
{
public:
    DocTreeViewItem (const ValueTree& tree,
                     FileTreeContainer* container,
                     ItemSorter* itemSorter);
    ~DocTreeViewItem ();

    /** Note 1: when this item is the root (project item of the top),
    this method will return this project's 'docs' dir,
    instead of the project file!

    Note 2: this method will return a nonexists file when the arg is invalid. */
    static const File getMdFileOrDir (const ValueTree& tree);

    /** these two methods will return index.html when it's a dir. */
    static const File getHtmlFileOrDir (const File& mdFileOrDir);
    static const File getHtmlFileOrDir (const ValueTree& tree);

    /** get a html-file's all local media files. the result will store in arg-2.
        return: media-files' number of this html-file. */
	static const int getHtmlMediaFiles (const File& htmlFile, Array<File>& files);

	/** 获取1参植树所代表的MD文件（目录）的媒体文件。所找到的媒体文件将保存在2参数组中。
	    
		注意：绝对不要在生成html网页文件的函数中调用此函数。因为此时html并未生成，
		其媒体文件尚不存在。而本函数内部使用 getHtmlMediaFiles() 来获取并转换媒体文件。	*/
	static const int getMdMediaFiles (const ValueTree& docOrDirTree, Array<File>& files);

    /** set the arg tree and all its parents to needCreateHtml. */
    static void needCreateAndUpload (const ValueTree& tree);

    // static public methods for drag-drop moving items..
    static void moveItems (const OwnedArray<ValueTree>& items, 
                           ValueTree newParent);

    void refreshDisplay ();
    const ValueTree& getTree () const    { return tree; }

    // override...
    virtual bool mightContainSubItems () override;
    virtual String getUniqueName () const override;
    virtual void itemOpennessChanged (bool isNowOpen) override;

    virtual String getTooltip () override;
    virtual void paintItem (Graphics& g, int width, int height) override;
    virtual void itemSelectionChanged (bool isNowSelected) override;
    virtual void itemClicked (const MouseEvent& e) override;

    // for move items..
    virtual var getDragSourceDescription () override;
    virtual bool isInterestedInDragSource (const DragAndDropTarget::SourceDetails& details) override;
    virtual void itemDropped (const DragAndDropTarget::SourceDetails& details,
                              int insertIndex) override;

    // draw line..
    virtual void paintHorizontalConnectingLine (Graphics&, const Line<float>& line) override;
    virtual void paintVerticalConnectingLine (Graphics&, const Line<float>& line) override;
    virtual void paintOpenCloseButton(Graphics&, const Rectangle<float>&, Colour, bool) override;
private:
    //=========================================================================
    /** export the selected item (all project-docs, a dir-docs or a doc) as a single md file. */
    static const bool exportDocsAsMd (DocTreeViewItem* item,
                                      const File& fileAppendTo);

    static DocTreeViewItem* getRootItem (DocTreeViewItem* subItem);

    //=========================================================================
    void menuPerform (const int menuIndex);

    // internal call the static method exportDocsAsMd()
    void exportAsMdFile ();
    void renameSelectedItem ();
    void createNewDocument ();
    void createNewFolder ();
    void deleteSelected ();

    //=========================================================================
    void valueTreePropertyChanged (ValueTree&, const Identifier&) override;
    void valueTreeChildAdded (ValueTree& parentTree, ValueTree&) override;
    void valueTreeChildRemoved (ValueTree& parentTree, ValueTree&, int) override;
    void valueTreeChildOrderChanged (ValueTree& parentTree, int, int) override;
    void valueTreeParentChanged (ValueTree&) override    { }
    void treeChildrenChanged (const ValueTree& parentTree);

    //=========================================================================
    ValueTree tree; // must NOT be refernce!!
    FileTreeContainer* treeContainer;
    ItemSorter* sorter = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DocTreeViewItem)

};


#endif  // HEADERGUA