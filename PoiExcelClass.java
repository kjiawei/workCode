package com.example.pkg;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.text.Format;
import java.util.ArrayList;
import java.util.Date;

import org.apache.poi.EncryptedDocumentException;
import org.apache.poi.hssf.extractor.ExcelExtractor;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.apache.poi.openxml4j.exceptions.InvalidFormatException;
import org.apache.poi.ss.usermodel.BorderStyle;
import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.CellType;
import org.apache.poi.ss.usermodel.CellValue;
import org.apache.poi.ss.usermodel.CreationHelper;
import org.apache.poi.ss.usermodel.DataFormat;
import org.apache.poi.ss.usermodel.DataFormatter;
import org.apache.poi.ss.usermodel.DateUtil;
import org.apache.poi.ss.usermodel.FillPatternType;
import org.apache.poi.ss.usermodel.Font;
import org.apache.poi.ss.usermodel.FormulaEvaluator;
import org.apache.poi.ss.usermodel.IndexedColors;
import org.apache.poi.ss.usermodel.RichTextString;
import org.apache.poi.ss.usermodel.Row;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;
import org.apache.poi.ss.usermodel.WorkbookFactory;
import org.apache.poi.ss.util.CellRangeAddress;
import org.apache.poi.xssf.extractor.XSSFExcelExtractor;
import org.apache.poi.xssf.usermodel.XSSFWorkbook;

public class PoiExcelClass {
/**
     * 筛选并重组表格(自动生成索引),只筛选第一个sheet
     * 
     * @params originFileName 源文件名
     * @params targetFileName 目标文件名
     * @params columns 只提取原表格中出现过的列(不包括索引)
     * @params filterItems 筛选条件,只提取出现该项的行
     */
    private static void filter(String originFileName, String targetFileName, String[] columns, String filterColumn, String[] filterItems) {
        InputStream inp = null;
        Workbook workbook = null;
        ArrayList<Integer> columnIndexList = new ArrayList<Integer>(columns.length);
        int filterColumnIndex = -1;
        Workbook workbook_w = null;
        FileOutputStream fos = null;
        try {
            inp = new FileInputStream(originFileName);
            workbook = WorkbookFactory.create(inp);
            FormulaEvaluator evaluator = workbook.getCreationHelper().createFormulaEvaluator();
            Sheet sheet = workbook.getSheetAt(0);
            Row row = sheet.getRow(0);
            // 如果未发现有行,则返回
            if (row == null)
                return;
            // 第一行规定必须是标题
            for (Cell cell : row) {
                if (cell.getCellTypeEnum() == CellType.STRING) {
                    for (String str : columns) {
                        String value = cell.getStringCellValue();
                        if (str.equals(value)) {
                            int index = cell.getColumnIndex();
                            columnIndexList.add(index);
                            if(filterColumnIndex == -1 && filterColumn.equals(value)) {
                                filterColumnIndex = index;
                            }
                            break;
                        }
                    }
                }
            }
            // 如果待筛选的字段一个也没找到就返回
            if (columnIndexList.size() == 0 || filterColumnIndex == -1)
                return;
            if (targetFileName.endsWith(SUFFIX_XSSF)) {
                workbook_w = new XSSFWorkbook();
            } else {
                workbook_w = new HSSFWorkbook();
            }
            Sheet sheet_w = workbook_w.createSheet("Sheet1");
            int rowIndex = 1;
            //添加标题行
            Row row_w = sheet_w.createRow(0);
            row_w.createCell(0).setCellValue("索引");
            for (int i = 0; i < columnIndexList.size(); i++) {
                String value = row.getCell(columnIndexList.get(i)).getStringCellValue();
                row_w.createCell(i+1).setCellValue(value);
            }
            for (Row r : sheet) {
                if(r.getRowNum() == 0) continue;
                Cell cell = r.getCell(filterColumnIndex);
                if(cell.getCellTypeEnum() == CellType.STRING) {
                     String value = cell.getStringCellValue();
                     for(String str : filterItems) {
                         if(str.equals(value)) {
                             //该行满足筛选条件
                             row_w = sheet_w.createRow(rowIndex);
                             row_w.createCell(0).setCellFormula("ROW()-1");
                             for (int i = 0; i < columnIndexList.size(); i++) {
                                 int index = columnIndexList.get(i);
                                 Cell c = r.getCell(index);
                                 Cell c_w = row_w.createCell(i+1, c.getCellType());
                                 //从原单元格中继承单元格格式,包括内容格式、边框、颜色、字体。注意:这里必须先create，不能直接设置其他单元格的格式
                                 CellStyle cs = workbook_w.createCellStyle();
                                 cs.cloneStyleFrom(c.getCellStyle());
                                 c_w.setCellStyle(cs);
                                 //以下是错误的方法,Error:This Style does not belong to the supplied Workbook Stlyes Source. Are you trying to assign a style from one workbook to the cell of a differnt workbook?
                                 //c_w.setCellStyle(c.getCellStyle());
                                 switch (c.getCellTypeEnum()) {
                                case _NONE:
                                    break;
                                case BLANK:
                                    break;
                                case BOOLEAN:
                                    c_w.setCellValue(c.getBooleanCellValue());
                                    break;
                                case ERROR:
                                    c_w.setCellErrorValue(c.getErrorCellValue());
                                    break;
                                case FORMULA:
                                    //唯一的问题就是公式由于是根据单元格计算,新表可能相应的单元格存放不同的数据,造成计算结果不同
//                                  c_w.setCellFormula(c.getCellFormula());
                                    CellValue cellValue = evaluator.evaluate(c);
                                    c_w.setCellType(cellValue.getCellType());
                                    switch (cellValue.getCellTypeEnum()) {
                                    case BOOLEAN:
                                        c_w.setCellValue(cellValue.getBooleanValue());
                                        break;
                                    case ERROR:
                                        c_w.setCellErrorValue(cellValue.getErrorValue());
                                        break;
                                    case NUMERIC:
                                        if(DateUtil.isCellDateFormatted(c)) {
                                            c_w.setCellValue(c.getDateCellValue());
                                        } else {
                                            c_w.setCellValue(c.getNumericCellValue());
                                        }
                                        break;
                                    case STRING:
                                        c_w.setCellValue(c.getRichStringCellValue());
                                        break;
                                    default:
                                        break;
                                    }
                                    break;
                                case NUMERIC:
                                    if(DateUtil.isCellDateFormatted(c)) {
                                        c_w.setCellValue(c.getDateCellValue());
                                    } else {
                                        c_w.setCellValue(c.getNumericCellValue());
                                    }
                                    break;
                                case STRING:
                                    c_w.setCellValue(c.getRichStringCellValue());
                                    break;
                                }
                             }
                             rowIndex++;
                             break;
                         }
                     }
                }
            }
            fos = new FileOutputStream(targetFileName);
            workbook_w.write(fos);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (EncryptedDocumentException e) {
            e.printStackTrace();
        } catch (InvalidFormatException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (inp != null) {
                try {
                    inp.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            if (workbook != null) {
                try {
                    workbook.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            if (workbook_w != null) {
                try {
                    workbook_w.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            if (fos != null) {
                try {
                    fos.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    public static void main(String[] args) {
        filter(EXCEL_PATH, "filter.xlsx", new String[] {"姓名","员工号","部门","年龄","入岗时间"}, "姓名", new String[] {"陈七", "张三", "林八", "钱二", "东方不败"});
    }